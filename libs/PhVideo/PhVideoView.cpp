/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhGraphic/PhGraphicText.h"

#include "PhVideoView.h"

PhVideoView::PhVideoView(QWidget *parent) :
	PhGraphicView(parent),
	_videoEngine(NULL),
	_maxVideoRate(0)
{
}

void PhVideoView::setEngine(PhVideoEngine *videoEngine)
{
	_videoEngine = videoEngine;
}

void PhVideoView::paint()
{
	if(_videoEngine) {
		int videoRate = _videoEngine->refreshRate();
		if(videoRate > _maxVideoRate)
			_maxVideoRate = videoRate;
		QString info = QString("%1 / %2").arg(videoRate).arg(_maxVideoRate);
		addInfo(info);
		_videoEngine->drawVideo(0, 0, this->width(), this->height());
		_videoEngine->clock()->tick(_screenFrequency);
	}
}
