/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICIMAGE_H
#define PHGRAPHICIMAGE_H

#include "PhGraphicTexturedRect.h"

class PhGraphicImage : public PhGraphicTexturedRect
{
public:
    /**
     * @brief PhGraphicImage
     * @param filename
     * @param x
     * @param y
     * @param z
     * @param w
     * @param h
     * @param color
     * @param tv
     * @param tu
     * Create a PhGraphicImage calling the PhGraphicTexturedRect constructor
     */
    PhGraphicImage(PhString filename, int x, int y, int z, int w, int h, PhColor color, int tv = 1, int tu = 1);
    void setFilename(PhString filename);
    PhString getFilename();

    /**
     * @brief draw
     * @param px
     * draw the PhGraphicImage using PhGraphicTexturedRect::draw()
     */
    void draw(int px);

    /**
     * @brief init
     * Currently unused
     */
    void init();
    /**
     * @brief dispose
     * Currently unused
     */
    void dispose();

private:

    PhString _filename;

    SDL_Surface * _surface;
};

#endif // PHGRAPHICIMAGE_H
