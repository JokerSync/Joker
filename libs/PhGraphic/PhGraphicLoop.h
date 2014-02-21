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
	 * @param crossHeight the height of the cross
	 * @param hThick the cross thickness
	 * @param horizontal if the graphicloop is horizontal or not
	 */
	PhGraphicLoop(int x = 0, int y = 0, int w = 0, int h = 0, int crossHeight = 0, int hThick = 0, bool horizontal = false);
	~PhGraphicLoop();

	/**
	 * @brief draw the PhGraphicLoop
	 */
	void draw();

	/**
	 * @brief Set the thickness
	 * @param hThick desired thickness
	 */
	void setHThick(int hThick);
	/**
	 * @brief Get the thickness
	 * @return
	 */
	int getHThick() {
		return _hThick;
	}

	/**
	 * @brief Set the cross height
	 * @param crossHeight
	 */
	void setCrossHeight(int crossHeight);

	/**
	 * @brief Set horizontal loop
	 *
	 * Set if the graphic loop is displayed horizontaly or not
	 * @param isHorizontal
	 */
	void setHorizontalLoop(bool isHorizontal) {
		_horizontal = isHorizontal;
	}
	/**
	 * @brief Get the cross height
	 * @return cross height in pixels
	 */
	int getCrossHeight() {
		return _crossHeight;
	}
private:

	bool _horizontal;
	int _crossHeight;
	int _hThick;

};
#endif // PHGRAPHICLOOP_H
