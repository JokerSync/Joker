#include "VideoStripView.h"

VideoStripView::VideoStripView(QWidget *parent) :
	PhGraphicView(parent)
{
}

bool VideoStripView::init()
{
	_strip.init();
}

void VideoStripView::paint()
{
	int videoHeight = this->height() * 3 / 4;
	int videoWidth = this->height();
	int videoX = (this->width() - videoWidth) / 2;
	_videoEngine.drawVideo(videoX, 0, videoWidth, videoHeight);
	_strip.draw(0, videoHeight, this->width(), this->height() - videoHeight);
}
