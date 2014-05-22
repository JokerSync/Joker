/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICLOOP_H
#define PHGRAPHICLOOP_H

#include "PhGraphicRect.h"

/**
 * @brief Draw the loop symbol (a cross)
 */
class PhGraphicLoop : public PhGraphicRect
{
public:

	/**
	 * @brief PhGraphicLoop constructor
	 * @param x Upper left corner coordinate
	 * @param y Upper left corner coordinate
	 * @param w Desired width
	 * @param h Desired height
	 * @param crossSize The cross size (width and height)
	 * @param thickness The loop elements thickness
	 * @param horizontal True if the loop layout is horizontal, false otherwise
	 */
	PhGraphicLoop(int x = 0, int y = 0, int w = 0, int h = 0, int crossSize = 0, int thickness = 0, bool horizontal = false);

	/**
	 * @brief draw the PhGraphicLoop
	 */
	void draw();

	/**
	 * @brief Set the thickness of the loop elements
	 * @param thickness An integer value
	 */
	void setThickness(int thickness);
	/**
	 * @brief Get the thickness of the loop elements
	 * @return An integer value
	 */
	int getThickness() {
		return _thickness;
	}

	/**
	 * @brief Set the cross size (width and height)
	 * @param crossSize An integer value
	 */
	void setCrossSize(int crossSize);
	/**
	 * @brief Get the cross size (width and height)
	 * @return An integer value
	 */
	int crossSize() {
		return _crossSize;
	}

	/**
	 * @brief Set horizontal loop layout
	 *
	 * Set if the loop is displayed horizontaly or not
	 *
	 * @param isHorizontal A boolean value
	 */
	void setHorizontalLoop(bool isHorizontal) {
		_isHorizontal = isHorizontal;
	}
private:
	int _crossSize;
	int _thickness;
	bool _isHorizontal;
};
#endif // PHGRAPHICLOOP_H
