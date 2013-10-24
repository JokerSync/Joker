#include "PhVideoEngine.h"

PhVideoEngine::PhVideoEngine(QObject *parent) :	QObject(parent),
	_settings(NULL),
	_clock(PhTimeCodeType25),
	_frameStamp(0),
	_pFormatContext(NULL),
	_videoStream(-1),
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
	return (_pFormatContext && (_videoStream >=0) && _pCodecContext && _pFrame);
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

	// Find video stream :
	for(int i = 0; i < _pFormatContext->nb_streams; i++)
	{
		if(_pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			_videoStream = i;
			break;
		}
	}

	if(_videoStream == -1)
		return false;

	_pCodecContext = _pFormatContext->streams[_videoStream]->codec;

	PHDEBUG << "size : " << _pCodecContext->width << "x" << _pCodecContext->height;
	AVCodec * pCodec = avcodec_find_decoder(_pCodecContext->codec_id);
	if(pCodec == NULL)
		return false;

	if(avcodec_open2(_pCodecContext, pCodec, NULL) < 0)
		return false;

	_pFrame = avcodec_alloc_frame();

	_clock.setFrame(0);
	bool result = goToFrame(0);
	PHDEBUG << "over : " << result;
	return result;
}

void PhVideoEngine::close()
{
	PHDEBUG;
	if(_rgb)
	{
		delete _rgb;
		_rgb = NULL;
	}
	if(_pFormatContext)
	{
		avformat_close_input(&_pFormatContext);
		_pFormatContext = NULL;
		_pCodecContext = NULL;
		_videoStream = -1;
	}
}

void PhVideoEngine::setSettings(QSettings *settings)
{
	_settings = settings;
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
	_clock.tick(60);
	PhFrame delay = 0;
	if(_settings)
		delay = _settings->value("delay", 0).toInt() * PhTimeCode::getFps(_clock.timeCodeType()) / 1000 * _clock.rate();
	goToFrame(_clock.frame() + delay);
	videoRect.setRect(x, y, w, h);
	videoRect.draw();
}

void PhVideoEngine::setFrameStamp(PhFrame frame)
{
	_frameStamp = frame;
}

PhVideoEngine::~PhVideoEngine()
{
	close();
}

bool PhVideoEngine::goToFrame(PhFrame frame)
{
	int lastGotoElapsed = _testTimer.elapsed();
	int seekElapsed = -1;
	int readElapsed = -1;
	int decodeElapsed = -1;
	int scaleElapsed = -1;
	int textureElapsed = -1;

	if(!ready())
		return false;

	if(frame < this->_frameStamp)
		frame = this->_frameStamp;
	if (frame >= this->_frameStamp + _pFormatContext->streams[_videoStream]->duration)
		frame = this->_frameStamp + _pFormatContext->streams[_videoStream]->duration;

	bool result = false;
	// Do not perform frame seek if the rate is 0 and the last frame is the same frame
	if (frame == _currentFrame)
		result = true;
	else
	{
		// Do not perform frame seek if the rate is 1 and the last frame is the previous frame
		if((_currentFrame < 0) || (_clock.rate() != 1) || (frame - _currentFrame != 1))
		{
			int flags = AVSEEK_FLAG_ANY;
			if(_clock.rate() < 0)
				flags |= AVSEEK_FLAG_BACKWARD;
			av_seek_frame(_pFormatContext, _videoStream, frame - this->_frameStamp, flags);
		}
		_currentFrame = frame;

		seekElapsed = _testTimer.elapsed();

		AVPacket packet;
		while(av_read_frame(_pFormatContext, &packet) >= 0)
		{
			if(packet.stream_index == _videoStream)
			{
				readElapsed = _testTimer.elapsed();
				int ok;
				avcodec_decode_video2(_pCodecContext, _pFrame, &ok, &packet);
				if(!ok)
					break;

				decodeElapsed = _testTimer.elapsed();

				_pSwsCtx = sws_getCachedContext(_pSwsCtx, _pFrame->width, _pCodecContext->height,
												_pCodecContext->pix_fmt, _pCodecContext->width, _pCodecContext->height,
												AV_PIX_FMT_RGBA, SWS_POINT, NULL, NULL, NULL);

				if(_rgb == NULL)
					_rgb = new uint8_t[_pFrame->width * _pFrame->height * 4];
				int linesize = _pFrame->width * 4;
				if (sws_scale(_pSwsCtx, (const uint8_t * const *) _pFrame->data,
							  _pFrame->linesize, 0, _pCodecContext->height, &_rgb,
							  &linesize) < 0)
					break;

				scaleElapsed = _testTimer.elapsed();

				videoRect.createTextureFromARGBBuffer(_rgb, _pFrame->width, _pFrame->height);

				textureElapsed = _testTimer.elapsed();

				_videoFrameTickCounter.tick();
				result = true;
				break;
			}
			av_free_packet(&packet); //important!
		}
	}

	int currentGotoElapsed = _testTimer.elapsed();
//	if(_testTimer.elapsed() > 25)
//		PHDEBUG << frame << lastGotoElapsed << seekElapsed - lastGotoElapsed << readElapsed - seekElapsed
//				<< decodeElapsed - readElapsed << scaleElapsed - decodeElapsed << textureElapsed - scaleElapsed << currentGotoElapsed - lastGotoElapsed << _testTimer.elapsed();
	_testTimer.restart();

	return result;
}
