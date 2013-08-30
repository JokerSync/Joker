#include "math.h"
#include "PhGraphicLoop.h"

PhGraphicLoop::PhGraphicLoop(float x, float y, float w, float h, float crossHeight, float hThick, float z, QColor *color) :
	PhGraphicRect(x, y, w, h, z, color), _crossHeight(crossHeight), _hThick(hThick)
{
}


bool PhGraphicLoop::init()
{
	return true;
}

void PhGraphicLoop::draw()
{
	//draw rect

	float x = _x - _hThick/2;
	float y = _y;
	float w = _hThick;
	float h = _h;

	glBegin(GL_QUADS); 	//Begining the cube's drawing
	{
		glVertex3f(x,		y,	_z);
		glVertex3f(x + w,	y,	_z);
		glVertex3f(x + w,	y + h,  _z);
		glVertex3f(x,		y + h,  _z);
	}
	glEnd();

	//draw cross

	x = _x - _w/2;
	y = _y + _h/2 - _crossHeight/2;
	w = _hThick;
	h = _crossHeight;

	glBegin(GL_QUADS); 	//Begining the cube's drawing
	{
		glVertex3f(x,		y,	_z);
		glVertex3f(x + w,	y,	_z);
		glVertex3f(x + _w,	y + h,  _z);
		glVertex3f(x + _w - w,		y + h,  _z);
	}
	glEnd();

	y = _y + _h/2 + _crossHeight/2;

	glBegin(GL_QUADS); 	//Begining the cube's drawing
	{
		glVertex3f(x,		y,	_z);
		glVertex3f(x + w,	y,	_z);
		glVertex3f(x + _w,	y - h,  _z);
		glVertex3f(x + _w -w,		y - h,  _z);
	}
	glEnd();
}

void PhGraphicLoop::setHThick(float hThick)
{
	_hThick = hThick;
}

void PhGraphicLoop::setCrossHeight(float crossHeight)
{
	_crossHeight = crossHeight;
}

