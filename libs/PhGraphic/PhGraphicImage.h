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

	PhGraphicImage( QString filename = NULL, int x = 0, int y =0, int w = 0, int h = 0, int z = 0, float tu = 1, float tv = 1, PhColor *color = new PhColor());

	void setFilename(QString filename);
	QString getFilename();

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

	QString _filename;

	SDL_Surface * _surface;
};

#endif // PHGRAPHICIMAGE_H
