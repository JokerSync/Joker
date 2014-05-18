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
	int x = this->getX() - _hThick / 2;
	int y = this->getY();
	int w = _hThick;
	int h = this->getHeight();

	glColor3f(this->color().redF(), this->color().greenF(), this->color().blueF());

	if(_horizontal) {
		w = this->getWidth();
		h = _hThick;
	}

	glBegin(GL_QUADS);  // draw the vertical line
	{
		glVertex3f(x,       y,  this->getZ());
		glVertex3f(x + w,   y,  this->getZ());
		glVertex3f(x + w,   y + h,  this->getZ());
		glVertex3f(x,       y + h,  this->getZ());
	}


	//draw cross



	if(_horizontal) {
		x = this->getX() + this->getWidth() / 2 - _crossHeight / 2;
		y = this->getY() - _crossHeight / 2;
		w = _hThick;
		h = _crossHeight;
#warning /// @todo Bad bad bad!!!!
		this->setWidth(_crossHeight);
	}
	else {
		x = this->getX() - this->getWidth()/2;
		y = this->getY() + this->getHeight()/2 - _crossHeight / 2;
		w = _hThick;
		h = _crossHeight;

	}
	glBegin(GL_QUADS);  // draw the fist cross segment
	{
		glVertex3f(x,           y,      this->getZ());
		glVertex3f(x + w,       y,      this->getZ());
		glVertex3f(x + this->getWidth(),      y + h,  this->getZ());
		glVertex3f(x + this->getWidth() - w,  y + h,  this->getZ());
	}

	glEnd();



	if(_horizontal) {
		y = this->getY() + _crossHeight / 2;

	}
	else {
		y = this->getY() + this->getHeight()/2 + _crossHeight/2;
	}
	glBegin(GL_QUADS);  // draw the second cross segment
	{
		glVertex3f(x,       y,  this->getZ());
		glVertex3f(x + w,   y,  this->getZ());
		glVertex3f(x + this->getWidth(),  y - h,  this->getZ());
		glVertex3f(x + this->getWidth() -w,       y - h,  this->getZ());
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

