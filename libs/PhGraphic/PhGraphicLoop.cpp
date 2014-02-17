/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "math.h"
#include "PhGraphicLoop.h"

PhGraphicLoop::PhGraphicLoop(int x, int y, int w, int h, int crossHeight, int hThick, bool horizontal) :
	PhGraphicRect(x, y, w, h), _crossHeight(crossHeight), _hThick(hThick), _horizontal(horizontal)
{
}
PhGraphicLoop::~PhGraphicLoop()
{
}

void PhGraphicLoop::draw()
{
	//draw rect
	int x = _x - _hThick / 2;
	int y = _y;
	int w = _hThick;
	int h = _h;

	glColor3f(_color.redF(), _color.greenF(), _color.blueF());

	if(_horizontal) {
		w = _w;
		h = _hThick;
	}

	glBegin(GL_QUADS);  // draw the vertical line
	{
		glVertex3f(x,       y,  _z);
		glVertex3f(x + w,   y,  _z);
		glVertex3f(x + w,   y + h,  _z);
		glVertex3f(x,       y + h,  _z);
	}


	//draw cross



	if(_horizontal) {
		x = _x + _w / 2 - _crossHeight / 2;
		y = _y - _crossHeight / 2;
		w = _hThick;
		h = _crossHeight;
		_w = _crossHeight;

	}
	else{
		x = _x - _w/2;
		y = _y + _h/2 - _crossHeight / 2;
		w = _hThick;
		h = _crossHeight;

	}
	glBegin(GL_QUADS);  // draw the fist cross segment
	{
		glVertex3f(x,           y,      _z);
		glVertex3f(x + w,       y,      _z);
		glVertex3f(x + _w,      y + h,  _z);
		glVertex3f(x + _w - w,  y + h,  _z);
	}

	glEnd();



	if(_horizontal) {
		y = _y + _crossHeight / 2;

	}
	else{
		y = _y + _h/2 + _crossHeight/2;
	}
	glBegin(GL_QUADS);  // draw the second cross segment
	{
		glVertex3f(x,       y,  _z);
		glVertex3f(x + w,   y,  _z);
		glVertex3f(x + _w,  y - h,  _z);
		glVertex3f(x + _w -w,       y - h,  _z);
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

