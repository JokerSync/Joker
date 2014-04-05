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
	glColor3f(_color.redF(), _color.greenF(), _color.blueF());

	int width = _w / (2 * _dashCount - 1);
	int x = _x;
	for(int i = 0; i < _dashCount; i++) {
		glBegin(GL_QUADS);
		{
			glVertex3i(x,      _y, _z);
			glVertex3i(x + width, _y, _z);
			glVertex3i(x + width, _y + _h,  _z);
			glVertex3i(x,      _y + _h,  _z);
		}
		x += 2 * width;
	}
	glEnd();
}
