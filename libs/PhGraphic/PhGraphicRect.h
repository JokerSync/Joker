/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICSQUARE_H
#define PHGRAPHICSQUARE_H

#include "PhGraphicObject.h"

class PhGraphicRect : public PhGraphicObject
{
public:

	PhGraphicRect(float x = 0, float y =0, float w = 0, float h = 0, float z = 0, QColor *color = new QColor());

    /**
     * @brief setSize
     * @param w
     * @param h
     * set the PhGraphicRect size using setHeight() and setWidht()
     */
    void setSize(float w, float h);
    /**
     * @brief setRect
	 * @param x
     * @param y
     * @param w
     * @param h
     * set the PhGraphicRect size and position usgin setSize() and setPosition()
     */
	void setRect(float x, float y, float w, float h);
    /**
     * @brief setHeight
     * @param h
     */
    void setHeight(float h);
    /**
     * @brief setWidht
     * @param w
     */
    void setWidth(float w);

    /**
     * @brief getHeight
     * @return _h
     */
    float getHeight();
    /**
     * @brief getWidth
     * @return _w
     */
    float getWidth();


protected:
    /**
     * @brief _w
     * is the PhGraphicRect width
     */
    float _w;
    /**
     * @brief _h
     * is the PhGraphicRect height
     */
    float _h;
};

#endif // PHGRAPHICSQUARE_H
