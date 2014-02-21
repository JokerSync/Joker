#include "VideoStripView.h"

VideoStripView::VideoStripView(QWidget *parent) :
	PhGraphicView(parent)
{
}

bool VideoStripView::init()
{
	connect(this, SIGNAL(beforePaint(PhTimeScale)), _strip.clock(), SLOT(tick(PhTimeScale)));

	_strip.init();
}

void VideoStripView::paint()
{
	int videoHeight = this->height() * 3 / 4;
	int videoWidth = this->height();
	int videoX = (this->width() - videoWidth) / 2;
	_strip.draw(0, videoHeight, this->width(), this->height() - videoHeight);
	_videoEngine.drawVideo(videoX, 0, videoWidth, videoHeight);
}
