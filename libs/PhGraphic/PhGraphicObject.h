/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICOBJECT_H
#define PHGRAPHICOBJECT_H

#include <QColor>
#include "PhTools/PhDebug.h"

class PhGraphicObject
{
public:

	PhGraphicObject(int x = 0, int y = 0);
	virtual ~PhGraphicObject();

	/**
	 * @brief init
	 */
	virtual bool init();
	/**
	 * @brief dispose
	 */
	virtual void dispose();

	/**
     * @brief draw
     * @param px
     * virtual method called to draw objects
     */
	virtual void draw() = 0;

    /**
     * @brief setPosition
     * @param x
     * @param y
     * @param z
     * set the position of the PhGraphicObject using setX() setY() setZ()
     */
	void setPosition(int x, int y, int z);
    /**
     * @brief setX
     * @param x
     */
    void setX(int x);
	/**
	 * @brief getX
	 * @return
	 */
	int getX();

	/**
     * @brief setY
     * @param y
     */
    void setY(int y);
	/**
	 * @brief getY
	 * @return
	 */
	int getY();

	/**
     * @brief setZ
     * @param z
     */
    void setZ(int z);

    /**
     * @brief getZ
     * @return
     */
    int getZ();

	void setColor(QColor color);

	QColor getColor();

protected:

    /**
     * @brief _x
     * is the PhGraphicObject horizontal position
     */
    int _x;
    /**
     * @brief _y
     * is the PhGraphicObject vertical position
     */
    int _y;
    /**
     * @brief _z
     * is the PhGrapicObject depth
     */
    int _z;

	/**
	 * @brief _color
	 * The PhColor of the PhGraphicObject
	 */
	QColor _color;
};

#endif // PHGRAPHICOBJECT_H
