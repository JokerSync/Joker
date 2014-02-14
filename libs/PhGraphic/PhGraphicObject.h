/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICOBJECT_H
#define PHGRAPHICOBJECT_H

#include <QColor>
#include <SDL2/SDL.h>

#if defined(Q_OS_WIN)
#include <GL/glu.h>
#else
#include <glu.h>
#endif

#include "PhTools/PhDebug.h"

/*!
 * \brief Provide an graphic OpenGL engine
 */
class PhGraphicObject
{
public:

	/*!
	 * \brief PhGraphicObject constructor
	 * \param x Upper left corner coordinates of the display object
	 * \param y Upper left corner coordinates of the displayed object
	 */
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

	/*!
	 * \brief Set the color of the object
	 * \param color the desired color
	 */
	void setColor(QColor color);

	/*!
	 * \brief Get the color of the object
	 * \return the color
	 */
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
