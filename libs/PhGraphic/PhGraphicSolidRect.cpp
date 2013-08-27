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
//		/*
//		(0,0) ------ (1,0)
//		  |            |
//		  |            |
//		(0,1) ------ (1,1)
//		*/

	glColor3f(_color->redF(), _color->greenF(), _color->blueF());
	glRectf(_x, _y, _x + _w, _y + _h);
}
