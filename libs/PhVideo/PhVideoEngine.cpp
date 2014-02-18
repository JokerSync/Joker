/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoEngine.h"

PhVideoEngine::PhVideoEngine(QObject *parent) : QObject(parent),
	_settings(NULL),
	_fileName(""),
	_clock(PhTimeCodeType25),
	_firstFrame(0),
	_pFormatContext(NULL),
	_videoStream(NULL),
	_pCodecContext(NULL),
	_pFrame(NULL),
	_pSwsCtx(NULL),
	_rgb(NULL),
	_currentFrame(-1)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();

	_testTimer.start();
}

bool PhVideoEngine::ready()
{
	return (_pFormatContext && _videoStream && _pCodecContext && _pFrame);
}

bool PhVideoEngine::open(QString fileName)
{
	close();
	PHDEBUG << fileName;
	if(avformat_open_input(&_pFormatContext, fileName.toStdString().c_str(), NULL, NULL) < 0)
		return false;

	// Retrieve stream information
	if (avformat_find_stream_info(_pFormatContext, NULL) < 0)
		return -1; // Couldn't find stream information

	av_dump_format(_pFormatContext, 0, fileName.toStdString().c_str(), 0);

	_firstFrame = 0;
	_videoStream = NULL;

	// Find video stream :
	for(int i = 0; i < (int)_pFormatContext->nb_streams; i++) {
		if(_pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			_videoStream = _pFormatContext->streams[i];
			break;
		}
	}

	if(_videoStream == NULL)
		return false;

	// Reading timestamp :
	AVDictionaryEntry *tag = av_dict_get(_pFormatContext->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);
	if(tag == NULL)
		tag = av_dict_get(_videoStream->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);

	if(tag) {
		PHDEBUG << "Found timestamp:" << tag->value;
		_firstFrame = PhTimeCode::frameFromString(tag->value, _clock.timeCodeType());
	}

	// Looking for timecode type
	float fps = this->framePerSecond();
	if(fps < 24)
		_clock.setTimeCodeType(PhTimeCodeType2398);
	else if (fps < 24.5f)
		_clock.setTimeCodeType(PhTimeCodeType24);
	else if (fps < 26)
		_clock.setTimeCodeType(PhTimeCodeType25);
	else
		_clock.setTimeCodeType(PhTimeCodeType2997);

	_pCodecContext = _videoStream->codec;

	PHDEBUG << "size : " << _pCodecContext->width << "x" << _pCodecContext->height;
	AVCodec * pCodec = avcodec_find_decoder(_pCodecContext->codec_id);
	if(pCodec == NULL)
		return false;

	if(avcodec_open2(_pCodecContext, pCodec, NULL) < 0)
		return false;

	_pFrame = avcodec_alloc_frame();

	PHDEBUG << "length:" << this->length();
	_currentFrame = -1;
	_clock.setFrame(0);
	goToFrame(0);
	_fileName = fileName;
	return true;
}

void PhVideoEngine::close()
{
	PHDEBUG;
	if(_rgb) {
		delete _rgb;
		_rgb = NULL;
	}

	if(_pFormatContext) {
		avformat_close_input(&_pFormatContext);
		_pFormatContext = NULL;
		_pCodecContext = NULL;
		_videoStream = NULL;
	}

	_fileName = "";
}

void PhVideoEngine::setSettings(QSettings *settings)
{
	_settings = settings;
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
//	_clock.tick(60);
	PhFrame delay = 0;
	if(_settings)
		delay = _settings->value("delay", 0).toInt() * PhTimeCode::getFps(_clock.timeCodeType()) * _clock.rate() / 1000;
	goToFrame(_clock.frame() + delay);
	videoRect.setRect(x, y, w, h);
	videoRect.setZ(-10);
	videoRect.draw();
}

PhFrame PhVideoEngine::length()
{
	if(_videoStream)
		return time2frame(_videoStream->duration);
	return 0;
}

void PhVideoEngine::setFirstFrame(PhFrame frame)
{
	_firstFrame = frame;
}

PhVideoEngine::~PhVideoEngine()
{
	close();
}

int PhVideoEngine::width()
{
	if(_pCodecContext)
		return _pCodecContext->width;
	return 0;
}

int PhVideoEngine::height()
{
	if(_pCodecContext)
		return _pCodecContext->height;
	return 0;
}

float PhVideoEngine::framePerSecond()
{
	float result = 0;
	if(_videoStream) {
		result = _videoStream->avg_frame_rate.num;
		result /= _videoStream->avg_frame_rate.den;
	}

	return result;
}

QString PhVideoEngine::codecName()
{
	if(_videoStream)
		return _videoStream->codec->codec_name;
	return "";
}

bool PhVideoEngine::goToFrame(PhFrame frame)
{
//	int lastGotoElapsed = _testTimer.elapsed();
	int seekElapsed = -1;
	int readElapsed = -1;
	int decodeElapsed = -1;
	int scaleElapsed = -1;
	int textureElapsed = -1;

	if(!ready()) {
		PHDEBUG << "not ready";
		return false;
	}

	if(frame < this->firstFrame())
		frame = this->firstFrame();
	if (frame >= this->lastFrame())
		frame = this->lastFrame();

	bool result = false;
	// Do not perform frame seek if the rate is 0 and the last frame is the same frame
	if (frame == _currentFrame)
		result = true;
	else{
		// Do not perform frame seek if the rate is 1 and the last frame is the previous frame
		if(frame - _currentFrame != 1) {
			int flags = AVSEEK_FLAG_ANY;
			int64_t timestamp = frame2time(frame - _firstFrame);
			PHDEBUG << "seek:" << timestamp << _videoStream->time_base.num << _videoStream->time_base.den;
			av_seek_frame(_pFormatContext, _videoStream->index, timestamp, flags);
		}

		seekElapsed = _testTimer.elapsed();

		AVPacket packet;

		bool lookingForVideoFrame = true;
		while(lookingForVideoFrame) {
			int error = av_read_frame(_pFormatContext, &packet);
			switch(error) {
			case 0:
				if(packet.stream_index == _videoStream->index) {
					_currentFrame = frame;

					readElapsed = _testTimer.elapsed();
					int frameFinished = 0;
					avcodec_decode_video2(_pCodecContext, _pFrame, &frameFinished, &packet);
					if(frameFinished) {
						decodeElapsed = _testTimer.elapsed();

						int frameHeight = _pFrame->height;
						if(_settings) {
							if(_settings->value("videoDeinterlace", false).toBool())
								frameHeight = _pFrame->height / 2;
						}
						_pSwsCtx = sws_getCachedContext(_pSwsCtx, _pFrame->width, _pCodecContext->height,
						                                _pCodecContext->pix_fmt, _pCodecContext->width, frameHeight,
						                                AV_PIX_FMT_RGBA, SWS_POINT, NULL, NULL, NULL);

						if(_rgb == NULL)
							_rgb = new uint8_t[_pFrame->width * frameHeight * 4];
						int linesize = _pFrame->width * 4;
						if (0 <= sws_scale(_pSwsCtx, (const uint8_t * const *) _pFrame->data,
						                   _pFrame->linesize, 0, _pCodecContext->height, &_rgb,
						                   &linesize)) {
							scaleElapsed = _testTimer.elapsed();

							videoRect.createTextureFromARGBBuffer(_rgb, _pFrame->width, frameHeight);

							textureElapsed = _testTimer.elapsed();

							_videoFrameTickCounter.tick();
							result = true;
						}
						lookingForVideoFrame = false;
					} // if frame decode is not finished, let's read another packet.
				}
				break;
			case AVERROR_INVALIDDATA:
			case AVERROR_EOF:
			default:
				{
					char errorStr[256];
					av_strerror(error, errorStr, 256);
					PHDEBUG << frame << "error:" << errorStr;
					lookingForVideoFrame = false;
					break;
				}
			}
			//Avoid memory leak
			av_free_packet(&packet);
		}
	}

//	int currentGotoElapsed = _testTimer.elapsed();
	//	if(_testTimer.elapsed() > 25)
	//		PHDEBUG << frame << lastGotoElapsed << seekElapsed - lastGotoElapsed << readElapsed - seekElapsed
	//				<< decodeElapsed - readElapsed << scaleElapsed - decodeElapsed << textureElapsed - scaleElapsed << currentGotoElapsed - lastGotoElapsed << _testTimer.elapsed();
	_testTimer.restart();

	return result;
}

int64_t PhVideoEngine::frame2time(PhFrame f)
{
	int64_t t = 0;
	if(_videoStream) {
		PhFrame fps = PhTimeCode::getFps(_clock.timeCodeType());
		t = f * _videoStream->time_base.den / _videoStream->time_base.num / fps;
	}
	return t;
}

PhFrame PhVideoEngine::time2frame(int64_t t)
{
	PhFrame f = 0;
	if(_videoStream) {
		PhFrame fps = PhTimeCode::getFps(_clock.timeCodeType());
		f = t * _videoStream->time_base.num * fps / _videoStream->time_base.den;
	}
	return f;
}
