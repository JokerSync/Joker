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
	PhGraphicObject::draw();
//		/*
//		(0,0) ------ (1,0)
//		  |            |
//		  |            |
//		(0,1) ------ (1,1)
//		*/

	glBegin(GL_QUADS);  //Begining the cube's drawing
	{
		glVertex3i(this->x(),      this->y(), this->z());
		glVertex3i(this->x() + this->width(), this->y(), this->z());
		glVertex3i(this->x() + this->width(), this->y() + this->height(),  this->z());
		glVertex3i(this->x(),      this->y() + this->height(),  this->z());
	}
	glEnd();
}
