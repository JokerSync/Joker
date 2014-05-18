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
	glColor3f(this->color().redF(), this->color().greenF(), this->color().blueF());

	int thickness = this->getHeight() / 10;
	int nose = this->getHeight() / 3;

	switch (_direction) {
	case DownLeftToUpRight:
		glBegin(GL_QUADS);
		{
			glVertex3i(this->getX(),      this->getY() + thickness, this->getZ());
			glVertex3i(this->getX()+ thickness,      this->getY(), this->getZ());
			glVertex3i(this->getX() + this->getWidth(), this->getY() + this->getHeight()- thickness,  this->getZ());
			glVertex3i(this->getX() + this->getWidth() - thickness, this->getY() + this->getHeight(),  this->getZ());
		}
		glEnd();

		glBegin(GL_TRIANGLES);
		{
			glVertex3i(this->getX() + this->getWidth(), this->getY() + this->getHeight(),  this->getZ());
			glVertex3i(this->getX() + this->getWidth() - nose, this->getY() + this->getHeight(),  this->getZ());
			glVertex3i(this->getX() + this->getWidth(), this->getY() + this->getHeight()- nose,  this->getZ());
		}
		glEnd();
		break;
	case UpLefToDownRight:
		glBegin(GL_QUADS);
		{
			glVertex3i(this->getX() + this->getWidth() -thickness, this->getY(), this->getZ());
			glVertex3i(this->getX(),      this->getY() + this->getHeight()-thickness,  this->getZ());
			glVertex3i(this->getX()+thickness,      this->getY() + this->getHeight(),  this->getZ());
			glVertex3i(this->getX() + this->getWidth(), this->getY()+thickness, this->getZ());
		}
		glEnd();
		glBegin(GL_TRIANGLES);
		{
			glVertex3i(this->getX() + this->getWidth(), this->getY(),  this->getZ());
			glVertex3i(this->getX() + this->getWidth() - nose, this->getY(),  this->getZ());
			glVertex3i(this->getX() + this->getWidth(), this->getY() + nose,  this->getZ());
		}
		glEnd();
		break;
	}
}
