/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QDebug>
#include "PhGraphicRect.h"



PhGraphicRect::PhGraphicRect(int x, int y, int w, int h, int z, QColor *color)
	: PhGraphicObject(x , y , z, color), _w(w), _h(h)
{
}

void PhGraphicRect::setSize(int w, int h)
{
    this->setHeight(h);
    this->setWidth(w);
}
void PhGraphicRect::setRect(int x, int y, int w, int h)
{
    this->setSize(w,h);
    this->setPosition(x,y,this->getZ());
}

void PhGraphicRect::setHeight(int h)
{
    _h = h;
}

void PhGraphicRect::setWidth(int w)
{
    _w = w;
}

int PhGraphicRect::getHeight()
{
    return _h;
}
int PhGraphicRect::getWidth()
{
    return _w;
}
