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

	glColor3f(this->color().redF(), this->color().greenF(), this->color().blueF());

	glBegin(GL_QUADS);  //Begining the cube's drawing
	{
		glVertex3i(this->getX(),      this->getY(), this->getZ());
		glVertex3i(this->getX() + this->getWidth(), this->getY(), this->getZ());
		glVertex3i(this->getX() + this->getWidth(), this->getY() + this->getHeight(),  this->getZ());
		glVertex3i(this->getX(),      this->getY() + this->getHeight(),  this->getZ());
	}
	glEnd();
}
