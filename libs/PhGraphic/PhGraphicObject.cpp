/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicObject.h"

PhGraphicObject::PhGraphicObject(float x , float y, float z, QColor* color) : _x(x), _y(y), _z(z), _color(color)
{
}

void PhGraphicObject::dispose()
{
}

void PhGraphicObject::setPosition(float x, float y, float z)
{
    this->setX(x);
    this->setY(y);
    this->setZ(z);
}


void PhGraphicObject::setX(float x){
    _x = x;
}

void PhGraphicObject::setY(float y){
    _y = y;
}

void PhGraphicObject::setZ(float z)
{
    _z = z;
}


float PhGraphicObject::getX(){
    return _x;
}

float PhGraphicObject::getY(){
    return _y;
}

float PhGraphicObject::getZ()
{
    return _z;
}


QColor * PhGraphicObject::getColor(){
	return _color;
}

void PhGraphicObject::setColor(QColor* color){
	_color = color;
}

