/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICTEXT_H
#define PHGRAPHICTEXT_H

#include <QDebug>

#include "glu.h"

#include "SDL_ttf/SDL_ttf.h"
#include "SDL_image/SDL_image.h"
//#include "SDL/SDL.h"

#include "PhGraphicTexturedRect.h"
#include "PhTools/PhString.h"


class PhGraphicText : PhGraphicTexturedRect {
public:
//    GLuint createTextureFromSurface(SDL_Surface * surface);
//    GLuint createSurfaceFromText(PhString text);
    PhGraphicText(PhString content, int x, int y, int z, int w, int h, PhFont *font, PhColor color);
    /**
     * @brief Draw the text on screen
     *
     */
    void draw(int px);
    void init();
    void dispose();

    void setContent(PhString content);
    void setFont(PhFont * font);

    PhString getContent();
    PhFont * getFont();

private:
    PhString _content;
    PhFont *_font;

};

#endif // PHGRAPHICTEXT_H
