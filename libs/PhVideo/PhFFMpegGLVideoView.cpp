#include "PhFFMpegGLVideoView.h"

PhFFMpegGLVideoView::PhFFMpegGLVideoView(QWidget *parent) :
	PhGraphicView(parent),
	_videoEngine(NULL)
{
}

void PhFFMpegGLVideoView::setEngine(PhVideoEngine *videoEngine)
{
	_videoEngine = videoEngine;
}

bool PhFFMpegGLVideoView::init()
{
}

void PhFFMpegGLVideoView::paint()
{
	if(_videoEngine)
		_videoEngine->drawVideo(0, 0, this->width(), this->height());
}
