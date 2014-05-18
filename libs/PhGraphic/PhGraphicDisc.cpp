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
		glVertex3f( this->x(), this->y(), this->z());
		for( int i = 0; i <= _resolution; i++ ) {
			float angle = i * 2 * M_PI / _resolution;
			glVertex3f( this->x() + sin( angle ) * _radius, this->y() + cos( angle ) * _radius, this->z());
		}
	}
	glEnd();
}
