#include "PhVideoView.h"

PhVideoView::PhVideoView(QWidget *parent) :
	PhGraphicView(parent),
	_videoEngine(NULL)
{
}

void PhVideoView::setEngine(PhVideoEngine *videoEngine)
{
	_videoEngine = videoEngine;
}

bool PhVideoView::init()
{
}

void PhVideoView::paint()
{
	if(_videoEngine)
		_videoEngine->drawVideo(0, 0, this->width(), this->height());
}
