/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICIMAGE_H
#define PHGRAPHICIMAGE_H

#include "SDL_image/SDL_image.h"

#include "PhGraphicTexturedRect.h"

class PhGraphicImage : public PhGraphicTexturedRect
{
public:
    PhGraphicImage(PhString filename, int x, int y, int z, int w, int h, PhColor color, int tv = 1, int tu = 1);
//    void setFilename(PhString filename);
//    PhString getFilename();


    void draw();

private:
    void init();
    void dispose();
    //PhString _filename;
};

#endif // PHGRAPHICIMAGE_H
