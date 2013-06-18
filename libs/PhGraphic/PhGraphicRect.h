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
    /**
     * @brief PhGraphicRect
     * @param x
     * @param y
     * @param z
     * @param w
     * @param h
     * @param color
     * Constructor calling PhGraphicObject::PhGraphicObject()
     */
    PhGraphicRect(int x, int y, int z, int w, int h, PhColor color);

    /**
     * @brief draw
     * @param px
     * draw the content to the frame buffer
     */
    void draw(int px);

    void init(){};
    void dispose(){};

    /**
     * @brief setSize
     * @param w
     * @param h
     * set the PhGraphicRect size using setHeight() and setWidht()
     */
    void setSize(int w, int h);
    /**
     * @brief setRect
     * @param x
     * @param y
     * @param w
     * @param h
     * set the PhGraphicRect size and position usgin setSize() and setPosition()
     */
    void setRect(int x, int y, int w, int h);
    /**
     * @brief setHeight
     * @param h
     */
    void setHeight(int h);
    /**
     * @brief setWidht
     * @param w
     */
    void setWidht(int w);

    /**
     * @brief getHeight
     * @return _h
     */
    int getHeight();
    /**
     * @brief getWidth
     * @return _w
     */
    int getWidth();


private:
    /**
     * @brief _w
     * is the PhGraphicRect width
     */
    int _w;
    /**
     * @brief _h
     * is the PhGraphicRect height
     */
    int _h;
};

#endif // PHGRAPHICSQUARE_H
