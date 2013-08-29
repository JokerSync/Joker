#include "PhGraphicSolidRect.h"

PhGraphicSolidRect::PhGraphicSolidRect(float x, float y, float w, float h, float z, QColor *color) :
	PhGraphicRect(x, y, w, h, z, color)
{
}


bool PhGraphicSolidRect::init()
{
	return true;
}

void PhGraphicSolidRect::draw()
{

//	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

//	glLoadIdentity();
//		/*
//		(0,0) ------ (1,0)
//		  |            |
//		  |            |
//		(0,1) ------ (1,1)
//		*/

	//qDebug() << " x:" << _x << " y:" << _y << " z:" << _z << " w:" << _w << " h:" << _h;

	glColor3f(_color->redF(), _color->greenF(), _color->blueF());
	//glRectf(_x, _y, _x + _w, _y + _h);
	glBegin(GL_QUADS); 	//Begining the cube's drawing
	{
		glVertex3f(_x,		_y,	_z);
		glVertex3f(_x + _w,	_y,	_z);
		glVertex3f(_x + _w,	_y + _h,  _z);
		glVertex3f(_x,		_y + _h,  _z);
	}
	glEnd();
}
