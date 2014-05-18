#include "PhGraphicDashedLine.h"

PhGraphicDashedLine::PhGraphicDashedLine(int dashCount, int x, int y, int w, int h) :
	PhGraphicRect(x, y, w, h),
	_dashCount(dashCount)
{
	if(_dashCount <= 0)
		_dashCount = 1;
}

void PhGraphicDashedLine::setDashCount(int dashCount)
{
	_dashCount = dashCount;
}

void PhGraphicDashedLine::draw()
{
	glColor3f(this->color().redF(), this->color().greenF(), this->color().blueF());

	int width = this->getWidth() / (2 * _dashCount - 1);
	int x = this->getX();
	for(int i = 0; i < _dashCount; i++) {
		glBegin(GL_QUADS);
		{
			glVertex3i(x,      this->getY(), this->getZ());
			glVertex3i(x + width, this->getY(), this->getZ());
			glVertex3i(x + width, this->getY() + this->getHeight(),  this->getZ());
			glVertex3i(x,      this->getY() + this->getHeight(),  this->getZ());
		}
		x += 2 * width;
	}
	glEnd();
}
