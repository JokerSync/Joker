#include "PhVideoEngine.h"

PhVideoEngine::PhVideoEngine(QObject *parent) :	QObject(parent),
	_clock(PhTimeCodeType25),
	_frameStamp(0),
	_pFormatContext(NULL),
	_videoStream(-1),
	_pCodecContext(NULL),
	_pFrame(NULL),
	_pSwsCtx(NULL),
	_rgb(NULL)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
}

bool PhVideoEngine::open(QString fileName)
{
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

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
	goToFrame(_clock.frame());
	videoRect.setRect(x, y, w, h);
	videoRect.draw();
}

void PhVideoEngine::setFrameStamp(PhFrame frame)
{
	_frameStamp = frame;
}

PhVideoEngine::~PhVideoEngine()
{
	if(_pFormatContext != NULL)
		avformat_close_input(&_pFormatContext);
}

bool PhVideoEngine::goToFrame(PhFrame frame)
{
	if(_videoStream < 0)
		return false;
	if(frame < this->_frameStamp)
		frame = this->_frameStamp;
	if (frame >= this->_frameStamp + _pFormatContext->streams[_videoStream]->duration)
		frame = this->_frameStamp + _pFormatContext->streams[_videoStream]->duration;
	av_seek_frame(_pFormatContext, _videoStream, frame - this->_frameStamp, 0);

	AVPacket packet;
	while(av_read_frame(_pFormatContext, &packet) >= 0)
	{
		if(packet.stream_index == _videoStream)
		{
			int ok;
			avcodec_decode_video2(_pCodecContext, _pFrame, &ok, &packet);
			if(!ok)
				return false;

			_pSwsCtx = sws_getCachedContext(_pSwsCtx, _pFrame->width, _pCodecContext->height,
										_pCodecContext->pix_fmt, _pCodecContext->width, _pCodecContext->height,
										AV_PIX_FMT_RGBA, SWS_POINT, NULL, NULL, NULL);

			if(_rgb == NULL)
				_rgb = new uint8_t[_pFrame->width * _pFrame->height * 4];
			int linesize = _pFrame->width * 4;
			if (sws_scale(_pSwsCtx, (const uint8_t * const *) _pFrame->data,
						  _pFrame->linesize, 0, _pCodecContext->height, &_rgb,
						  &linesize) < 0)
				return false;

			videoRect.createTextureFromARGBBuffer(_rgb, _pFrame->width, _pFrame->height);

			return true;
		}
	}
	return false;
}
