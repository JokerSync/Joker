#include "math.h"
#include "PhGraphicLoop.h"

PhGraphicLoop::PhGraphicLoop(float x, float y, float w, float h, float th, float z, QColor *color) :
	PhGraphicRect(x, y, w, h, z, color), th(th)
{
}


bool PhGraphicLoop::init()
{
	return true;
}

void PhGraphicLoop::draw()
{
	//draw rect

	x = this->getX()-th/2;
	y = this->getY();
	w = this->getTh();
	h = this->getHeight();

	glBegin(GL_QUADS); 	//Begining the cube's drawing
	{
		glVertex3f(x,		y,	z);
		glVertex3f(x + w,	y,	z);
		glVertex3f(x + w,	y + h,  z);
		glVertex3f(x,		y + h,  z);
	}
	glEnd();

	//draw cross

	x = this->getX()-this->getWidth()/2;
	y = this->getY()+this->getHeight()/2-this->getWidth()/2;
	w = this->getTh();
	h = this->getWidth();

	glBegin(GL_QUADS); 	//Begining the cube's drawing
	{
		glVertex3f(x,		y,	z);
		glVertex3f(x + w,	y,	z);
		glVertex3f(x + h,	y + h,  z);
		glVertex3f(x +h -w,		y + h,  z);
	}
	glEnd();

	y = this->getY()+this->getHeight()/2+this->getWidth()/2;

	glBegin(GL_QUADS); 	//Begining the cube's drawing
	{
		glVertex3f(x,		y,	z);
		glVertex3f(x + w,	y,	z);
		glVertex3f(x + h,	y - h,  z);
		glVertex3f(x +h -w,		y - h,  z);
	}
	glEnd();
}

void PhGraphicLoop::setTh(float newTh)
{
	th = newTh;
}
