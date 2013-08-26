/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICOBJECT_H
#define PHGRAPHICOBJECT_H

#include <QColor>

#include "PhFont.h"

class PhGraphicObject
{
public:

	PhGraphicObject(float x = 0, float y = 0, float z = 0, QColor * color = new QColor(1,1,1,1));

	/**
	 * @brief init
	 */
	virtual bool init() = 0;
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
	void setPosition(float x, float y, float z);
    /**
     * @brief setX
     * @param x
     */
    void setX(float x);
	/**
	 * @brief getX
	 * @return
	 */
	float getX();

	/**
     * @brief setY
     * @param y
     */
    void setY(float y);
	/**
	 * @brief getY
	 * @return
	 */
	float getY();

	/**
     * @brief setZ
     * @param z
     */
    void setZ(float z);

    /**
     * @brief getZ
     * @return
     */
    float getZ();

	void setColor(QColor *color);

	QColor *getColor();

protected:

    /**
     * @brief _x
     * is the PhGraphicObject horizontal position
     */
    float _x;
    /**
     * @brief _y
     * is the PhGraphicObject vertical position
     */
    float _y;
    /**
     * @brief _z
     * is the PhGrapicObject depth
     */
    float _z;

	/**
	 * @brief _color
	 * The PhColor of the PhGraphicObject
	 */
	QColor* _color;
};

#endif // PHGRAPHICOBJECT_H
