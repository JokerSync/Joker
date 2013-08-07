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

	PhGraphicImage( PhString filename = NULL, int x = 0, int y =0, int w = 0, int h = 0, int z = 0, int tu = 1, int tv = 1);

	void setFilename(PhString filename);
    PhString getFilename();

    /**
     * @brief draw
     * @param px
     * draw the PhGraphicImage using PhGraphicTexturedRect::draw()
     */
	void draw();

    /**
     * @brief init
     * Currently unused
     */
	bool init();
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
