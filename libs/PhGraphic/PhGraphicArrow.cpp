#include "PhGraphicArrow.h"

PhGraphicArrow::PhGraphicArrow(PhGraphicArrow::PhGraphicArrowDirection direction, int x, int y, int w, int h)
	: PhGraphicRect(x, y, w, h),
	  _direction(direction)

{
}

void PhGraphicArrow::setDirection(PhGraphicArrow::PhGraphicArrowDirection direction)
{
	_direction = direction;
}

void PhGraphicArrow::draw()
{
	glColor3f(_color.redF(), _color.greenF(), _color.blueF());

	int thickness = _h / 10;
	int nose = _h / 3;

	switch (_direction) {
	case DownLeftToUpRight:
		glBegin(GL_QUADS);
		{
			glVertex3i(_x,      _y + thickness, _z);
			glVertex3i(_x+ thickness,      _y , _z);
			glVertex3i(_x + _w , _y + _h- thickness,  _z);
			glVertex3i(_x + _w - thickness, _y + _h,  _z);
		}
		glEnd();

		glBegin(GL_TRIANGLES);
		{
			glVertex3i(_x + _w, _y + _h,  _z);
			glVertex3i(_x + _w - nose, _y + _h,  _z);
			glVertex3i(_x + _w , _y + _h- nose,  _z);
		}
		glEnd();
  	break;
	case UpLefToDownRight:
		glBegin(GL_QUADS);
		{
			glVertex3i(_x + _w -thickness, _y, _z);
			glVertex3i(_x,      _y + _h-thickness,  _z);
			glVertex3i(_x+thickness,      _y + _h,  _z);
			glVertex3i(_x + _w, _y+thickness, _z);
		}
		glEnd();
		glBegin(GL_TRIANGLES);
		{
			glVertex3i(_x + _w, _y,  _z);
			glVertex3i(_x + _w - nose, _y,  _z);
			glVertex3i(_x + _w , _y + nose,  _z);
		}
		glEnd();
		break;
	}
}
