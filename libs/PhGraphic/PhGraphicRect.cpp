/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QDebug>
#include "PhGraphicRect.h"



PhGraphicRect::PhGraphicRect(float x, float y, float w, float h, float z, QColor *color)
	: PhGraphicObject(x , y , z, color), _w(w), _h(h)
{
}

void PhGraphicRect::setSize(float w, float h)
{
    this->setHeight(h);
    this->setWidth(w);
}
void PhGraphicRect::setRect(float x, float y, float w, float h)
{
    this->setSize(w,h);
    this->setPosition(x,y,this->getZ());
}

void PhGraphicRect::setHeight(float h)
{
    _h = h;
}

void PhGraphicRect::setWidth(float w)
{
    _w = w;
}

float PhGraphicRect::getHeight()
{
    return _h;
}
float PhGraphicRect::getWidth()
{
    return _w;
}
