/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhGraphicStripView.h"

PhGraphicStripView::PhGraphicStripView(QWidget *parent) :
	PhGraphicView(parent), _strip(parent)
{
}

bool PhGraphicStripView::init()
{
	connect(this, SIGNAL(beforePaint(PhTimeScale)), _strip.clock(), SLOT(tick(PhTimeScale)));

	return _strip.init();
}

void PhGraphicStripView::paint()
{
	_strip.draw(0, 0, this->width(), this->height());
}
