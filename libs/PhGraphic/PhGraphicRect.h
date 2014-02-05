/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICSQUARE_H
#define PHGRAPHICSQUARE_H

#include <QtGlobal>

#include "PhGraphicObject.h"

class PhGraphicRect : public PhGraphicObject
{
public:

	PhGraphicRect(int x = 0, int y =0, int w = 0, int h = 0);
	~PhGraphicRect();

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
    void setWidth(int w);

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


protected:
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
