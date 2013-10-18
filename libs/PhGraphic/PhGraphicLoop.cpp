#include "math.h"
#include "PhGraphicLoop.h"

PhGraphicLoop::PhGraphicLoop(int x, int y, int w, int h, int crossHeight, int hThick) :
	PhGraphicRect(x, y, w, h), _crossHeight(crossHeight), _hThick(hThick)
{
}

void PhGraphicLoop::draw()
{
	//draw rect
	int x = _x - _hThick/2;
	int y = _y;
	int w = _hThick;
	int h = _h;

	glColor3f(_color.redF(), _color.greenF(), _color.blueF());


	glBegin(GL_QUADS); 	// draw the vertical line
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

	glBegin(GL_QUADS); 	// draw the fist cross segment
	{
		glVertex3f(x,		y,	_z);
		glVertex3f(x + w,	y,	_z);
		glVertex3f(x + _w,	y + h,  _z);
		glVertex3f(x + _w - w,		y + h,  _z);
	}
	glEnd();

	y = _y + _h/2 + _crossHeight/2;

	glBegin(GL_QUADS); 	// draw the second cross segment
	{
		glVertex3f(x,		y,	_z);
		glVertex3f(x + w,	y,	_z);
		glVertex3f(x + _w,	y - h,  _z);
		glVertex3f(x + _w -w,		y - h,  _z);
	}
	glEnd();



}

void PhGraphicLoop::setHThick(int hThick)
{
	_hThick = hThick;
}

void PhGraphicLoop::setCrossHeight(int crossHeight)
{
	_crossHeight = crossHeight;
}

