/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoEngine.h"

PhVideoEngine::PhVideoEngine(PhVideoSettings *settings) :
	_settings(settings),
	_fileName(""),
	_clock(PhTimeCodeType25),
	_firstFrame(0),
	_pFormatContext(NULL),
	_videoStream(NULL),
	_videoFrame(NULL),
	_pSwsCtx(NULL),
	_rgb(NULL),
	_currentFrame(PHFRAMEMIN),
	_useAudio(false),
	_audioStream(NULL),
	_audioFrame(NULL),
	_deinterlace(false),
	_bilinearFiltering(true)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
	avcodec_register_all();

	_testTimer.start();
}

bool PhVideoEngine::ready()
{
	return (_pFormatContext && _videoStream && _videoFrame);
}

void PhVideoEngine::setDeinterlace(bool deinterlace)
{
	_deinterlace = deinterlace;
	_currentFrame = PHFRAMEMIN;
	if(_rgb) {
		delete _rgb;
		_rgb = NULL;
	}
}

void PhVideoEngine::setBilinearFiltering(bool bilinear)
{
	if (_bilinearFiltering != bilinear) {
		_bilinearFiltering = bilinear;
		videoRect.setBilinearFiltering(bilinear);
	}
}

bool PhVideoEngine::open(QString fileName)
{
	close();
	PHDEBUG << fileName;
	if(avformat_open_input(&_pFormatContext, fileName.toStdString().c_str(), NULL, NULL) < 0)
		return false;

	// Retrieve stream information
	if (avformat_find_stream_info(_pFormatContext, NULL) < 0)
		return false; // Couldn't find stream information

	av_dump_format(_pFormatContext, 0, fileName.toStdString().c_str(), 0);

	_firstFrame = 0;
	_videoStream = NULL;
	_audioStream = NULL;

	// Find video stream :
	for(int i = 0; i < (int)_pFormatContext->nb_streams; i++) {
		AVMediaType streamType = _pFormatContext->streams[i]->codec->codec_type;
		PHDEBUG << i << ":" << streamType;
		switch(streamType) {
		case AVMEDIA_TYPE_VIDEO:
			_videoStream = _pFormatContext->streams[i];
			PHDEBUG << "\t=> video";
			break;
		case AVMEDIA_TYPE_AUDIO:
			if(_useAudio && (_audioStream == NULL))
				_audioStream = _pFormatContext->streams[i];
			PHDEBUG << "\t=> audio";
			break;
		default:
			PHDEBUG << "\t=> unknown";
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
	if(fps == 0) {
		PHDEBUG << "Bad fps detect => assuming 25";
		_clock.setTimeCodeType(PhTimeCodeType25);
	}
	else if(fps < 24)
		_clock.setTimeCodeType(PhTimeCodeType2398);
	else if (fps < 24.5f)
		_clock.setTimeCodeType(PhTimeCodeType24);
	else if (fps < 26)
		_clock.setTimeCodeType(PhTimeCodeType25);
	else if (fps < 30)
		_clock.setTimeCodeType(PhTimeCodeType2997);
	else {
#warning /// @todo patch for #107 => find better fps decoding
		PHDEBUG << "Bad fps detect => assuming 25";
		_clock.setTimeCodeType(PhTimeCodeType25);
	}

	PHDEBUG << "size : " << _videoStream->codec->width << "x" << _videoStream->codec->height;
	AVCodec * videoCodec = avcodec_find_decoder(_videoStream->codec->codec_id);
	if(videoCodec == NULL) {
		PHDEBUG << "Unable to find the codec:" << _videoStream->codec->codec_id;
		return false;
	}


	if (avcodec_open2(_videoStream->codec, videoCodec, NULL) < 0) {
		PHDEBUG << "Unable to open the codec:" << _videoStream->codec;
		return false;
	}

	_videoFrame = avcodec_alloc_frame();

	PHDEBUG << "length:" << this->length();
	PHDEBUG << "fps:" << this->framePerSecond();
	_currentFrame = PHFRAMEMIN;
	_clock.setFrame(0);

	if(_audioStream) {
		AVCodec* audioCodec = avcodec_find_decoder(_audioStream->codec->codec_id);
		if(audioCodec) {
			if(avcodec_open2(_audioStream->codec, audioCodec, NULL) < 0) {
				PHDEBUG << "Unable to open audio codec.";
				_audioStream = NULL;
			}
			else {
				_audioFrame = avcodec_alloc_frame();
				PHDEBUG << "Audio OK.";
			}
		}
		else {
			PHDEBUG << "Unable to find codec for audio.";
			_audioStream = NULL;
		}
	}

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
		_videoStream = NULL;
	}

	_fileName = "";
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
	//	_clock.tick(60);
	PhFrame delay = 0;
	if(_settings)
		delay = _settings->screenDelay() * PhTimeCode::getFps(_clock.timeCodeType()) * _clock.rate() / 1000;
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
	if(_videoStream)
		return _videoStream->codec->width;
	return 0;
}

int PhVideoEngine::height()
{
	if(_videoStream)
		return _videoStream->codec->height;
	return 0;
}

float PhVideoEngine::framePerSecond()
{
	float result = 0;
	if(_videoStream) {
		result = _videoStream->avg_frame_rate.num;
		result /= _videoStream->avg_frame_rate.den;
		// See http://stackoverflow.com/a/570694/2307070
		// for NaN handling
		if(result != result) {
			result = _videoStream->time_base.den;
			result /= _videoStream->time_base.num;
		}
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
	else {
		// Do not perform frame seek if the rate is 1 and the last frame is the previous frame
		if(frame - _currentFrame != 1) {
			int flags = AVSEEK_FLAG_ANY;
			int64_t timestamp = frame2time(frame - _firstFrame);
			PHDEBUG << "seek:" << frame;
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
					avcodec_decode_video2(_videoStream->codec, _videoFrame, &frameFinished, &packet);
					if(frameFinished) {
						decodeElapsed = _testTimer.elapsed();

						int frameHeight = _videoFrame->height;
						if(_deinterlace)
							frameHeight = _videoFrame->height / 2;

						// As the following formats are deprecated (see https://libav.org/doxygen/master/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)
						// we replace its with the new ones recommended by LibAv
						// in order to get ride of the warnings
						AVPixelFormat pixFormat;
						switch (_videoStream->codec->pix_fmt) {
						case AV_PIX_FMT_YUVJ420P:
							pixFormat = AV_PIX_FMT_YUV420P;
							break;
						case AV_PIX_FMT_YUVJ422P:
							pixFormat = AV_PIX_FMT_YUV422P;
							break;
						case AV_PIX_FMT_YUVJ444P:
							pixFormat = AV_PIX_FMT_YUV444P;
							break;
						case AV_PIX_FMT_YUVJ440P:
							pixFormat = AV_PIX_FMT_YUV440P;
						default:
							pixFormat = _videoStream->codec->pix_fmt;
							break;
						}
						/* Note: we output the frames in AV_PIX_FMT_BGRA rather than AV_PIX_FMT_RGB24,
						 * because this format is native to most video cards and will avoid a conversion
						 * in the video driver */
						_pSwsCtx = sws_getCachedContext(_pSwsCtx,
						                                _videoFrame->width, _videoStream->codec->height, pixFormat,
						                                _videoStream->codec->width, frameHeight, AV_PIX_FMT_BGRA,
						                                SWS_POINT, NULL, NULL, NULL);

						if(_rgb == NULL)
							_rgb = new uint8_t[avpicture_get_size(AV_PIX_FMT_BGRA, _videoFrame->width, frameHeight)];
						int linesize = _videoFrame->width * 4;
						if (0 <= sws_scale(_pSwsCtx, (const uint8_t * const *) _videoFrame->data,
						                   _videoFrame->linesize, 0, _videoStream->codec->height, &_rgb,
						                   &linesize)) {
							scaleElapsed = _testTimer.elapsed();

							videoRect.createTextureFromBGRABuffer(_rgb, _videoFrame->width, frameHeight);

							textureElapsed = _testTimer.elapsed();

							_videoFrameTickCounter.tick();
							result = true;
						}
						lookingForVideoFrame = false;
					} // if frame decode is not finished, let's read another packet.
				}
				else if(_audioStream && (packet.stream_index == _audioStream->index)) {
					int ok = 0;
					avcodec_decode_audio4(_audioStream->codec, _audioFrame, &ok, &packet);
					if(ok) {
						PHDEBUG << "audio:" << _audioFrame->nb_samples;
					}
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
