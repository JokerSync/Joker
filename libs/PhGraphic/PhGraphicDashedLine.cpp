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
	PhGraphicRect::draw();

	int width = this->width() / (2 * _dashCount - 1);
	int x = this->x();
	for(int i = 0; i < _dashCount; i++) {
		glBegin(GL_QUADS);
		{
			glVertex3i(x,      this->y(), this->z());
			glVertex3i(x + width, this->y(), this->z());
			glVertex3i(x + width, this->y() + this->height(),  this->z());
			glVertex3i(x,      this->y() + this->height(),  this->z());
		}
		x += 2 * width;
	}
	glEnd();
}
