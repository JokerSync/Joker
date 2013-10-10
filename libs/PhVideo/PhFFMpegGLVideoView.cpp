#include "PhFFMpegGLVideoView.h"

PhFFMpegGLVideoView::PhFFMpegGLVideoView(QWidget *parent) :
	PhGraphicView(parent),
	_pSwsCtx(NULL),
	_rgb(NULL)
{
	connect(&_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
}

void PhFFMpegGLVideoView::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	goToFrame(frame);
}

void PhFFMpegGLVideoView::onRateChanged(PhRate rate)
{
	// DO NOTHING
}

void PhFFMpegGLVideoView::checkVideoPosition()
{
	// DO NOTHING
}

bool PhFFMpegGLVideoView::drawFrame(AVFrame *avFrame)
{
	if(_pSwsCtx == NULL)
	{
		_pSwsCtx = sws_getContext(avFrame->width, _pCodecContext->height,
								_pCodecContext->pix_fmt, _pCodecContext->width, _pCodecContext->height,
								AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	}

	if(_rgb == NULL)
		_rgb = new uint8_t[avFrame->width * avFrame->height * 3];
	int linesize = avFrame->width * 3;
	if (sws_scale(_pSwsCtx, (const uint8_t * const *) avFrame->data,
				  avFrame->linesize, 0, _pCodecContext->height, &_rgb,
				  &linesize) < 0)
		return false;

	videoRect.createTextureFromRGBBuffer(_rgb, avFrame->width, avFrame->height);

	return true;
}

bool PhFFMpegGLVideoView::init()
{
}

void PhFFMpegGLVideoView::paint()
{
	videoRect.setRect(0, 0, this->width() /2, this->height()/ 2);
	videoRect.draw();
}
