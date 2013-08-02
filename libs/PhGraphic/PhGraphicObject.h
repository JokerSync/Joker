/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICOBJECT_H
#define PHGRAPHICOBJECT_H

#include "PhTools/PhFont.h"
#include "PhTools/PhColor.h"

class PhGraphicObject
{
public:

    /**
     * @brief draw
     * @param px
     * virtual method called to draw objects
     */
    virtual void draw(int px) = 0;

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
     * @brief setY
     * @param y
     */
    void setY(int y);
    /**
     * @brief setZ
     * @param z
     */
    void setZ(int z);
    /**
     * @brief setColor
     * @param color
     * Set the PhColor of the PhGraphicObject
     */
    void setColor(PhColor color);

    /**
     * @brief getColor
     * @return
     */
    PhColor getColor();

protected:
    /**
     * @brief init
     */
   // virtual void init() = 0;
    /**
     * @brief dispose
     */
  //  virtual void dispose() = 0;
    /**
     * @brief getX
     * @return
     */
    int getX();
    /**
     * @brief getY
     * @return
     */
    int getY();
    /**
     * @brief getZ
     * @return
     */
    int getZ();

private:
    /**
     * @brief _color
     * The PhColor of the PhGraphicObject
     */
    PhColor _color;
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
};

#endif // PHGRAPHICOBJECT_H
