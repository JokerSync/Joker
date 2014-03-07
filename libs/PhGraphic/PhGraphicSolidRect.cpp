/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhGraphicSolidRect.h"

PhGraphicSolidRect::PhGraphicSolidRect(int x, int y, int w, int h) :
	PhGraphicRect(x, y, w, h)
{
}

PhGraphicSolidRect::~PhGraphicSolidRect()
{
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

	glBegin(GL_QUADS);  //Begining the cube's drawing
	{
		glVertex3i(_x,      _y, _z);
		glVertex3i(_x + _w, _y, _z);
		glVertex3i(_x + _w, _y + _h,  _z);
		glVertex3i(_x,      _y + _h,  _z);
	}
	glEnd();
}
