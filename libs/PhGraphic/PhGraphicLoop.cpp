/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "math.h"
#include "PhGraphicLoop.h"

PhGraphicLoop::PhGraphicLoop(int x, int y, int w, int h, int crossSize, int thickness, bool isHorizontal) :
	PhGraphicRect(x, y, w, h),
	_crossSize(crossSize),
	_thickness(thickness),
	_isHorizontal(isHorizontal)
{
}

void PhGraphicLoop::draw()
{
	PhGraphicRect::draw();

	int x = this->x() - _thickness / 2;
	int y = this->y();
	int z = this->z();
	int w = _thickness;
	int h = this->height();

	if(_isHorizontal) {
		x = this->x();
		y = this->y() - _thickness / 2;
		w = this->width();
		h = _thickness;
	}

	// Draw the main rectangle
	glBegin(GL_QUADS);  // draw the vertical line
	{
		glVertex3i(x,     y,     z);
		glVertex3i(x + w, y,     z);
		glVertex3i(x + w, y + h, z);
		glVertex3i(x,     y + h, z);

		x = this->x();
		y = this->y() + this->height() / 2;

		if(_isHorizontal) {
			x = this->x() + this->width() / 2;
			y = this->y();
		}

		int hcs = _crossSize / 2; // half cross size
		int ht = _thickness / 3; // half thickness;

		// draw the fist cross segment
		glVertex3i(x - hcs + ht, y - hcs - ht, z);
		glVertex3i(x - hcs - ht, y - hcs + ht, z);
		glVertex3i(x + hcs - ht, y + hcs + ht, z);
		glVertex3i(x + hcs + ht, y + hcs - ht, z);

		// draw the second cross segment
		glVertex3i(x + hcs - ht, y - hcs - ht, z);
		glVertex3i(x + hcs + ht, y - hcs + ht, z);
		glVertex3i(x - hcs + ht, y + hcs + ht, z);
		glVertex3i(x - hcs - ht, y + hcs - ht, z);
	}
	glEnd();
}

void PhGraphicLoop::setThickness(int thickness)
{
	_thickness = thickness;
}

void PhGraphicLoop::setCrossSize(int crossSize)
{
	_crossSize = crossSize;
}

