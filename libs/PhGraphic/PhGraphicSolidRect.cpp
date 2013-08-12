#include "PhGraphicSolidRect.h"

PhGraphicSolidRect::PhGraphicSolidRect(int x, int y, int w, int h, int z, PhColor *color) :
	PhGraphicRect(x, y, w, h, z, color)
{
}


bool PhGraphicSolidRect::init()
{
}


void PhGraphicSolidRect::dispose()
{
}


void PhGraphicSolidRect::draw()
{


	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 	// Clear the  framebuffer & the depthbuffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	int x = this->getX();
	int w = this->getWidth();


		glBindTexture(GL_TEXTURE_2D, _texture);
		if (w == 240)
			glDisable(GL_BLEND);

		glEnable(GL_TEXTURE_2D);



		int y = this->getY();
		int h = this->getHeight();
		int z = this->getZ();

		/*
		(0,0) ------ (1,0)
		  |            |
		  |            |
		(0,1) ------ (1,1)
		*/

		//qDebug() << " x:" << x << " y:" << y << " z:" << z << " w:" << w << " h:" << h << " tu:" << _tu << " tv:" << _tv;

		glBegin(GL_QUADS); 	//Begining the cube's drawing
		{
			glTexCoord3i(0, 0, 1);glVertex3i(x ,         y,      z);
			glTexCoord3i(_tv, 0, 1);glVertex3i(x + w * _tv ,     y,      z);
			glTexCoord3i(_tv, _tu, 1);glVertex3i(x + w * _tv ,     y + h * _tu,  z);
			glTexCoord3i(0, _tu, 1);glVertex3i(x ,         y + h * _tu,  z);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
}
