/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhGraphicStripView.h"

PhGraphicStripView::PhGraphicStripView(QWidget *parent) :
	PhGraphicView(parent), _strip(parent)
{
}

bool PhGraphicStripView::init()
{
	return _strip.init();
}

void PhGraphicStripView::paint()
{
	_strip.draw(0, 0, this->width(), this->height());
}
