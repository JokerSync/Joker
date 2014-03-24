/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhGraphicStripView.h"

PhGraphicStripView::PhGraphicStripView(QWidget *parent) :
	PhGraphicView(parent), _strip(parent), _settings(NULL)
{
}

void PhGraphicStripView::setSettings(PhGraphicStripSettings *settings)
{
	_settings = settings;
	_strip.setSettings(settings);
}

bool PhGraphicStripView::init()
{
	connect(this, SIGNAL(beforePaint(PhTimeScale)), _strip.clock(), SLOT(tick(PhTimeScale)));

	return _strip.init();
}

void PhGraphicStripView::paint()
{
	int h = this->height();
	if(_settings)
		h = this->height()* _settings->stripHeight();
	_strip.draw(0, this->height() - h, this->width(), h);

	PhGraphicText text(_strip.getHUDFont(), "", 0, 0, 200, 100);
	text.setContent(QString("Drop : %1").arg(_strip.dropDetected()));
	text.draw();
	text.setY(100);
	text.setContent(QString("Max : %1").arg(_strip.maxDrawElapsed()));
	text.draw();
}
