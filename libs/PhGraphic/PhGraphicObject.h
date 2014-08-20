/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICOBJECT_H
#define PHGRAPHICOBJECT_H

#include <QColor>
#include <SDL2/SDL.h>

/* QtGui provides OpenGL definitions */
#include <QtGui>

#include "PhTools/PhDebug.h"

/**
 * @brief A generic graphic object
 *
 * A PhGraphicObject is described by :
 * - its position (x, y, z)
 * - its color
 * Concrete PhGraphicObject must implement the draw() method.
 */
class PhGraphicObject
{
public:

	/**
	 * @brief PhGraphicObject constructor
	 * @param x Upper left corner coordinates of the display object
	 * @param y Upper left corner coordinates of the displayed object
	 */
	PhGraphicObject(int x = 0, int y = 0);
	virtual ~PhGraphicObject();

	/**
	 * @brief dispose
	 */
	virtual void dispose();

	/**
	 * @brief Draw the graphical object
	 *
	 * Since the object is virtual, it only set the color.
	 */
	virtual void draw();

	/**
	 * @brief setPosition
	 * @param x
	 * @param y
	 * @param z
	 * set the position of the PhGraphicObject using setX() setY() setZ()
	 */
	void setPosition(int x, int y, int z);
	/**
	 * @brief Set the object X coordinate
	 * @param x An integer value
	 */
	void setX(int x);
	/**
	 * @brief Get the object X coordinate
	 * @return An integer value
	 */
	int x();

	/**
	 * @brief Set the object Y coordinate
	 * @param y An integer value
	 */
	void setY(int y);

	/**
	 * @brief Get the object y coordinate
	 * @return An integer value
	 */
	int y();

	/**
	 * @brief Set the object z coordinate
	 * @param z An integer value
	 */
	void setZ(int z);

	/**
	 * @brief Get the object z coordinate
	 * @return An integer value
	 */
	int z();

	/**
	 * @brief Set the object color
	 * @param color A color value
	 */
	void setColor(QColor color);

	/**
	 * @brief Get the color of the object
	 * @return the color
	 */
	QColor color();

protected:
	/**
	 * @brief Initialize the graphic object
	 */
	virtual bool init();

	/**
	 * @brief Check if the graphic object is initialized
	 * @return A boolean value
	 */
	bool ready();

private:

	int _x, _y, _z;
	QColor _color;
	bool _ready;
};

#endif // PHGRAPHICOBJECT_H
