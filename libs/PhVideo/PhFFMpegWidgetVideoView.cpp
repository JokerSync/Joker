#include <QPainter>

#include "PhFFMpegWidgetVideoView.h"


PhFFMpegWidgetVideoView::PhFFMpegWidgetVideoView(QWidget *parent) :
	QWidget(parent),
	_image(NULL),
	_rgb(NULL)
{
	connect(&_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
}

void PhFFMpegWidgetVideoView::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	goToFrame(frame);
}

void PhFFMpegWidgetVideoView::onRateChanged(PhRate rate)
{

}

void PhFFMpegWidgetVideoView::checkVideoPosition()
{

}

bool PhFFMpegWidgetVideoView::drawFrame(AVFrame *avFrame)
{
	if(_image == NULL)
		resizeEvent(NULL);
	int linesize = _image->width() * 3;
	// Convert the image into YUV format that SDL uses
	if (sws_scale(_pSwsCtx, (const uint8_t * const *) avFrame->data,
				 avFrame->linesize, 0, _pCodecContext->height, &_rgb,
				 &linesize) < 0)
		return false;
	this->update();
	return true;
}

void PhFFMpegWidgetVideoView::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.fillRect(0, 0, this->width(), this->height(), Qt::black);

	if(_image)
		painter.drawImage(0, 0, *_image);
}

void PhFFMpegWidgetVideoView::resizeEvent(QResizeEvent *event)
{
	PHDEBUG;

	if(_pCodecContext == NULL)
		return;

	int w = this->width();
	int h = this->height();

	// adjust width to a multiple of 4:
	int pow = 4;
	if(w % pow)
		w += pow - (w % pow);

	if(_rgb)
		delete _rgb;
	_rgb = new uint8_t[3 * w * h];

	if(_image)
		delete _image;

	_image = new QImage(_rgb, w, h, QImage::Format_RGB888);

	if(_pSwsCtx)
		delete _pSwsCtx;
	_pSwsCtx = sws_getContext(_pCodecContext->width, _pCodecContext->height,
							_pCodecContext->pix_fmt, w, h,
							AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
	goToFrame(_clock.frame());
	PHDEBUG << "ok";
}
