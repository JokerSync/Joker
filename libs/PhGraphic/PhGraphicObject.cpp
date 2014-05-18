/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhGraphicObject.h"

PhGraphicObject::PhGraphicObject(int x, int y) :
	_x(x),
	_y(y),
	_z(0),
	_color(Qt::white),
	_ready(false)
{
}

PhGraphicObject::~PhGraphicObject()
{
}

void PhGraphicObject::dispose()
{
}

void PhGraphicObject::draw()
{
	glColor3f(_color.redF(), _color.greenF(), _color.blueF());
}

bool PhGraphicObject::init()
{
	_ready = true;
	return _ready;
}

bool PhGraphicObject::ready()
{
	return _ready;
}

void PhGraphicObject::setPosition(int x, int y, int z)
{
	this->setX(x);
	this->setY(y);
	this->setZ(z);
}


void PhGraphicObject::setX(int x) {
	_x = x;
}

void PhGraphicObject::setY(int y) {
	_y = y;
}

void PhGraphicObject::setZ(int z)
{
	_z = z;
}


int PhGraphicObject::x() {
	return _x;
}

int PhGraphicObject::y() {
	return _y;
}

int PhGraphicObject::z()
{
	return _z;
}


QColor PhGraphicObject::color() {
	return _color;
}

void PhGraphicObject::setColor(QColor color) {
	_color = color;
}

