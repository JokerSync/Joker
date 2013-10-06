#include "PhFFMpegVideoView.h"

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
}

PhFFMpegVideoView::PhFFMpegVideoView(QWidget *parent) :
	PhGraphicView(parent), _pFormatContext(NULL)
{
	qDebug() << "Using FFMpeg widget for video playback.";
	av_register_all();

	connect(&_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(&_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
}

bool PhFFMpegVideoView::open(QString fileName)
{
	if(avformat_open_input(&_pFormatContext, fileName.toStdString().c_str(), NULL, NULL) == 0)
	{
		av_dump_format(_pFormatContext, 0, fileName.toStdString().c_str(), 0);

		int videoStream = -1;
		// Find video stream :
		for(int i = 0; i < _pFormatContext->nb_streams; i++)
		{
			if(_pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				videoStream = i;
				break;
			}
		}

		if(videoStream != -1)
		{
			AVCodecContext * pCodecContext = _pFormatContext->streams[videoStream]->codec;

			qDebug() << "size : " << pCodecContext->width << "x" << pCodecContext->height;
			AVCodec * pCodec = avcodec_find_decoder(pCodecContext->codec_id);
			if(pCodec != NULL)
			{
				if(avcodec_open2(pCodecContext, pCodec, NULL) >= 0)
				{
					_pFrame = avcodec_alloc_frame();
					AVPacket packet;
					while(av_read_frame(_pFormatContext, &packet) >= 0)
					{
						if(packet.stream_index == videoStream)
						{
							int ok;
							avcodec_decode_video2(pCodecContext, _pFrame, &ok, &packet);
							if(ok)
							{
								qDebug() << "pixel format" << (_pFrame->format == AV_PIX_FMT_YUVJ422P);
								_videoRect.createTextureFromYUVBuffer(_pFrame->data[0], _pFrame->linesize[0], pCodecContext->height);
							}
							break;
						}
					}
				}
			}
		}
	}
	return false;
}

PhFFMpegVideoView::~PhFFMpegVideoView()
{
	if(_pFormatContext != NULL)
		avformat_close_input(&_pFormatContext);
}

bool PhFFMpegVideoView::init()
{
	_videoRect.setSize(this->width(), this->height());
}

void PhFFMpegVideoView::paint()
{
	_videoRect.draw();
}

void PhFFMpegVideoView::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
}

void PhFFMpegVideoView::onRateChanged(PhRate rate)
{
}

void PhFFMpegVideoView::checkVideoPosition()
{

}
