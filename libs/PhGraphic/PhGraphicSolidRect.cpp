#include "PhGraphicSolidRect.h"

PhGraphicSolidRect::PhGraphicSolidRect(int x, int y, int w, int h, int z, QColor *color) :
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
	int x = this->getX();
	int w = this->getWidth();//largeur

	int y = this->getY();
	int h = this->getHeight();//hauteur
	int z = this->getZ();

//		/*
//		(0,0) ------ (1,0)
//		  |            |
//		  |            |
//		(0,1) ------ (1,1)
//		*/

//qDebug() << " x:" << x << " y:" << y << " z:" << z << " w:" << w << " h:" << h ;
	glColor3d(this->getColor()->redF(), this->getColor()->greenF(), this->getColor()->blueF());
	glRectd( x, y, x+w, y+h);
}
