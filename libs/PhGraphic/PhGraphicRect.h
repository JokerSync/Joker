/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICSQUARE_H
#define PHGRAPHICSQUARE_H

#include <QSize>

#include "PhGraphicObject.h"

/**
 * @brief A generic rectangle object
 *
 * A PhGraphicRect is a PhGraphicObject with a size (height and width).
 */
class PhGraphicRect : public PhGraphicObject
{
public:

	/**
	 * @brief PhGraphicRect constructor
	 * @param x The x coordinate
	 * @param y The y coordinate
	 * @param w The width
	 * @param h The height
	 */
	PhGraphicRect(int x = 0, int y = 0, int w = 0, int h = 0);
	~PhGraphicRect();

	/**
	 * @brief Set the rectangle size
	 * @param size The size
	 */
	void setSize(QSize size);

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
	 * @brief Get the rectangle height
	 * @return The height
	 */
	int height();

	/**
	 * @brief Set the rectangle width
	 * @param w The width
	 */
	void setWidth(int w);

	/**
	 * @brief Get the rectangle width
	 * @return The width
	 */
	int width();


private:
	int _w, _h;
};

#endif // PHGRAPHICSQUARE_H
