/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICCONTEXT_H
#define PHGRAPHICCONTEXT_H

#include <QDebug>
#include <QImage>

#include "glu.h"

#include "SDL/SDL.h"
#include "SDL_ttf/SDL_ttf.h"

#include "PhTools/PhString.h"

class PhGraphicView;

class PhGraphicContext
{
public:
    /**
     * @brief PhGraphicContext
     * @param view
     * Constructor
     */
    PhGraphicContext(PhGraphicView *view);
    /**
     * @brief saveToPNG
     * Save the current framebuffer to a PNG file
     */
    void saveToPNG();
    /**
     * @brief saveToPNG
     * @param number
     * Save the current framebuffer and the (number - 1) nexts elements
     */
    void saveToPNG(PhString number);

private:
    /**
     * @brief init
     * initialisation of the graphic context (SDL, TTF and OpenGL)
     */
    void init();
    /**
     * @brief dispose
     *  dispose the graphic context (SDL, TTF and OpenGL)
     */
    void dispose();

    PhGraphicView *_graphVeiw;

};

#endif // PHGRAPHICCONTEXT_H
