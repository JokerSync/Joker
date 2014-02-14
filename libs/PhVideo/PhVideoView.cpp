/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

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
	return true;
}

void PhVideoView::paint()
{
	if(_videoEngine) {
		_videoEngine->drawVideo(0, 0, this->width(), this->height());
		_videoEngine->clock()->tick(60);
	}
}
