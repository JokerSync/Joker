/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "math.h"
#include "PhGraphicLoop.h"

PhGraphicLoop::PhGraphicLoop(int x, int y, int w, int h, int crossHeight, int hThick, bool horizontal) :
	PhGraphicRect(x, y, w, h),
	_crossHeight(crossHeight),
	_hThick(hThick),
	_horizontal(horizontal)
{
}
PhGraphicLoop::~PhGraphicLoop()
{
}

void PhGraphicLoop::draw()
{
	//draw rect
	int x = this->x() - _hThick / 2;
	int y = this->y();
	int w = _hThick;
	int h = this->height();

	glColor3f(this->color().redF(), this->color().greenF(), this->color().blueF());

	if(_horizontal) {
		w = this->width();
		h = _hThick;
	}

	glBegin(GL_QUADS);  // draw the vertical line
	{
		glVertex3f(x,       y,  this->z());
		glVertex3f(x + w,   y,  this->z());
		glVertex3f(x + w,   y + h,  this->z());
		glVertex3f(x,       y + h,  this->z());
	}


	//draw cross



	if(_horizontal) {
		x = this->x() + this->width() / 2 - _crossHeight / 2;
		y = this->y() - _crossHeight / 2;
		w = _hThick;
		h = _crossHeight;
#warning /// @todo Bad bad bad!!!!
		this->setWidth(_crossHeight);
	}
	else {
		x = this->x() - this->width()/2;
		y = this->y() + this->height()/2 - _crossHeight / 2;
		w = _hThick;
		h = _crossHeight;

	}
	glBegin(GL_QUADS);  // draw the fist cross segment
	{
		glVertex3f(x,           y,      this->z());
		glVertex3f(x + w,       y,      this->z());
		glVertex3f(x + this->width(),      y + h,  this->z());
		glVertex3f(x + this->width() - w,  y + h,  this->z());
	}

	glEnd();



	if(_horizontal) {
		y = this->y() + _crossHeight / 2;

	}
	else {
		y = this->y() + this->height()/2 + _crossHeight/2;
	}
	glBegin(GL_QUADS);  // draw the second cross segment
	{
		glVertex3f(x,       y,  this->z());
		glVertex3f(x + w,   y,  this->z());
		glVertex3f(x + this->width(),  y - h,  this->z());
		glVertex3f(x + this->width() -w,       y - h,  this->z());
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

