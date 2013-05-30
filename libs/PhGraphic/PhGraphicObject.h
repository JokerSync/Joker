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
    PhGraphicObject(PhColor color, int x, int y, int z);
    virtual void draw() = 0;

    void setPosition(int x, int y, int z);
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    void setColor(PhColor color);

    PhColor getColor();

protected:
    virtual void init() = 0;
    virtual void dispose() = 0;
    int getX();
    int getY();
    int getZ();

private:
    PhColor _color;
    int _x;
    int _y;
    int _z;
};

#endif // PHGRAPHICOBJECT_H
