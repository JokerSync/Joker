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
	_strip.draw(0, 0, this->width(), this->height() / 2);
	_videoEngine.drawVideo(0, this->height() / 2, this->width(), this->height() / 2);
}
