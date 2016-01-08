/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICSOLIDSQUARE_H
#define PHGRAPHICSOLIDSQUARE_H

#include "PhGraphicRect.h"

/**
 * @brief A graphic rect object
 *
 * This graphic rectangle is filled by an solid color and specified by
 * a color, position and size (inherited from PhGraphicRect)
 */
class PhGraphicSolidRect : public PhGraphicRect
{
public:
	/**
	 * @brief PhGraphicSolidRect constructor
	 * @param x The x coordinate
	 * @param y The y coordinate
	 * @param w The width
	 * @param h The height
	 */
	PhGraphicSolidRect(int x = 0, int y = 0, int w = 0, int h = 0);

	/**
	 * @brief Draw a solid graphic rectangle
	 */
	void draw();

};

#endif // PHGRAPHICSOLIDSQUARE_H
