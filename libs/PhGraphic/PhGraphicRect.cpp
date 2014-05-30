/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhGraphicRect.h"

PhGraphicRect::PhGraphicRect(int x, int y, int w, int h)
	: PhGraphicObject(x, y), _w(w), _h(h)
{
}

PhGraphicRect::~PhGraphicRect()
{
}

void PhGraphicRect::setSize(QSize size)
{
	this->setWidth(size.width());
	this->setHeight(size.height());
}

void PhGraphicRect::setSize(int w, int h)
{
	this->setWidth(w);
	this->setHeight(h);
}

void PhGraphicRect::setRect(int x, int y, int w, int h)
{
	this->setSize(w, h);
	this->setPosition(x, y, this->z());
}

void PhGraphicRect::setHeight(int h)
{
	_h = h;
}

void PhGraphicRect::setWidth(int w)
{
	_w = w;
}

int PhGraphicRect::height()
{
	return _h;
}
int PhGraphicRect::width()
{
	return _w;
}
