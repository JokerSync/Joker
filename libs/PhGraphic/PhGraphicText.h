/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICTEXT_H
#define PHGRAPHICTEXT_H

#include <QDebug>

#include "glu.h"

#include "SDL_ttf.h"
#include "SDL_image.h"

#include "PhGraphicTexturedRect.h"
#include "PhTools/PhString.h"


class PhGraphicText : PhGraphicTexturedRect {
public:
//    GLuint createTextureFromSurface(SDL_Surface * surface);
//    GLuint createSurfaceFromText(PhString text);
    /**
     * @brief PhGraphicText
     * @param content
     * @param x
     * @param y
     * @param z
     * @param w
     * @param h
     * @param font
     * @param color
     * Constructor using PhGraphicTexturedRect::PhGraphicTexturedRect()
     */
    PhGraphicText(PhString content, int x, int y, int z, int w, int h, PhFont *font, PhColor color);
    /**
     * @brief Draw the text on screen
     *
     */
    void draw(int px);
    /**
     * @brief init
     * currently unused
     */
    void init();
    /**
     * @brief dispose
     * currently unused
     */
    void dispose();

    /**
     * @brief setContent
     * @param content
     * Set the PhGraphicText content
     */
    void setContent(PhString content);
    /**
     * @brief setFont
     * @param font
     * Set the PhGraphicText font
     */
    void setFont(PhFont * font);

    /**
     * @brief getContent
     * @return _content
     */
    PhString getContent();
    /**
     * @brief getFont
     * @return _font
     */
    PhFont * getFont();

private:
    /**
     * @brief _content
     */
    PhString _content;
    /**
     * @brief _font
     */
    PhFont *_font;

};

#endif // PHGRAPHICTEXT_H
