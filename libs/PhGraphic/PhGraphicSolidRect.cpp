#include "PhGraphicSolidRect.h"

PhGraphicSolidRect::PhGraphicSolidRect(int x, int y, int w, int h) :
	PhGraphicRect(x, y, w, h)
{
}


bool PhGraphicSolidRect::init()
{
	return true;
}

void PhGraphicSolidRect::draw()
{
//		/*
//		(0,0) ------ (1,0)
//		  |            |
//		  |            |
//		(0,1) ------ (1,1)
//		*/

	glColor3f(_color.redF(), _color.greenF(), _color.blueF());

	glBegin(GL_QUADS); 	//Begining the cube's drawing
	{
		glVertex3i(_x,		_y,	_z);
		glVertex3i(_x + _w,	_y,	_z);
		glVertex3i(_x + _w,	_y + _h,  _z);
		glVertex3i(_x,		_y + _h,  _z);
	}
	glEnd();
}
