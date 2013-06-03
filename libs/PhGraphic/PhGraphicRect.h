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
    PhGraphicRect(int x, int y, int z, int w, int h, PhColor color);

    void draw(int px);

    void setSize(int w, int h);
    void setRect(int x, int y, int w, int h);

    void setHeight(int h);
    void setWidht(int w);

    int getHeight();
    int getWidth();


private:
    int _w;
    int _h;
};

#endif // PHGRAPHICSQUARE_H
