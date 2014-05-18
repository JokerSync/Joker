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
	PhGraphicObject::draw();

	int thickness = this->height() / 10;
	int nose = this->height() / 3;

	switch (_direction) {
	case DownLeftToUpRight:
		glBegin(GL_QUADS);
		{
			glVertex3i(this->x(),      this->y() + thickness, this->z());
			glVertex3i(this->x()+ thickness,      this->y(), this->z());
			glVertex3i(this->x() + this->width(), this->y() + this->height()- thickness,  this->z());
			glVertex3i(this->x() + this->width() - thickness, this->y() + this->height(),  this->z());
		}
		glEnd();

		glBegin(GL_TRIANGLES);
		{
			glVertex3i(this->x() + this->width(), this->y() + this->height(),  this->z());
			glVertex3i(this->x() + this->width() - nose, this->y() + this->height(),  this->z());
			glVertex3i(this->x() + this->width(), this->y() + this->height()- nose,  this->z());
		}
		glEnd();
		break;
	case UpLefToDownRight:
		glBegin(GL_QUADS);
		{
			glVertex3i(this->x() + this->width() -thickness, this->y(), this->z());
			glVertex3i(this->x(),      this->y() + this->height()-thickness,  this->z());
			glVertex3i(this->x()+thickness,      this->y() + this->height(),  this->z());
			glVertex3i(this->x() + this->width(), this->y()+thickness, this->z());
		}
		glEnd();
		glBegin(GL_TRIANGLES);
		{
			glVertex3i(this->x() + this->width(), this->y(),  this->z());
			glVertex3i(this->x() + this->width() - nose, this->y(),  this->z());
			glVertex3i(this->x() + this->width(), this->y() + nose,  this->z());
		}
		glEnd();
		break;
	}
}
