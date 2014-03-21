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
	_maxGraphicRate(0),
	_maxVideoRate(0)
{
}

void PhVideoView::setEngine(PhVideoEngine *videoEngine)
{
	_videoEngine = videoEngine;
}

bool PhVideoView::init()
{
	_font.setFontFile("/Library/Fonts/Arial.ttf");
	return true;
}

void PhVideoView::paint()
{
	if(_videoEngine) {
		int graphicRate = this->refreshRate();
		if(graphicRate > _maxGraphicRate)
			_maxGraphicRate = graphicRate;
		int videoRate = _videoEngine->refreshRate();
		if(videoRate > _maxVideoRate)
			_maxVideoRate = videoRate;
		QString info = QString("%1 / %2 - %3 / %4 - %5 / %6")
				.arg(graphicRate)
				.arg(_maxGraphicRate)
				.arg(videoRate)
				.arg(_maxVideoRate)
				.arg(_videoEngine->bufferOccupation())
				.arg(100);
		PhGraphicText text(&_font, info, 0, 0, 400, 100);
		text.setColor(Qt::red);
		_videoEngine->drawVideo(0, 0, this->width(), this->height());
		text.draw();
		_videoEngine->clock()->tick(60);
	}
}
