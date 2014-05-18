#include <QtMath>

#include "PhGraphicDisc.h"

PhGraphicDisc::PhGraphicDisc(int x, int y, int radius, int resolution)
	: PhGraphicObject(x, y),
	_radius(radius),
	_resolution(resolution)
{

}

void PhGraphicDisc::draw()
{
	glBegin(GL_TRIANGLE_FAN);
	{
		glVertex3f( this->getX(), this->getY(), this->getZ());
		for( int i = 0; i <= _resolution; i++ ) {
			float angle = i * 2 * M_PI / _resolution;
			glVertex3f( this->getX() + sin( angle ) * _radius, this->getY() + cos( angle ) * _radius, this->getZ());
		}
	}
	glEnd();
}
