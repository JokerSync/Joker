/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICSQUARE_H
#define PHGRAPHICSQUARE_H

#include "PhGraphicObject.h"

/**
 * @brief A generic rectangle object
 *
 * A PhGraphicRect is a PhGraphicObject with a size (height and width).
 */
class PhGraphicRect : public PhGraphicObject
{
public:

	PhGraphicRect(int x = 0, int y = 0, int w = 0, int h = 0);
	~PhGraphicRect();

	/**
	 * @brief Set the rectangle size
	 * @param w The width
	 * @param h The height
	 */
	void setSize(int w, int h);
	/**
	 * @brief Set the rectangle position and size
	 * @param x The x coordinate
	 * @param y The y coordinate
	 * @param w The width
	 * @param h The height
	 */
	void setRect(int x, int y, int w, int h);
	/**
	 * @brief Set the rectangle height
	 * @param h The height
	 */
	void setHeight(int h);
	/**
	 * @brief Set the rectangle width
	 * @param w The width
	 */
	void setWidth(int w);

	/**
	 * @brief Get the rectangle height
	 * @return The height
	 */
	int getHeight();
	/**
	 * @brief Get the rectangle width
	 * @return The width
	 */
	int getWidth();


protected:
	/** @brief The rectangle width */
	int _w;
	/** @brief The rectangle height */
	int _h;
};

#endif // PHGRAPHICSQUARE_H
