/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICTEXTUREDSQUARE_H
#define PHGRAPHICTEXTUREDSQUARE_H

#include <QDebug>

#include "glu.h"

#include "SDL_image/SDL_image.h"
#include "SDL/SDL.h"

#include "PhTools/PhString.h"

#include "PhGraphicRect.h"

class PhGraphicTexturedRect : PhGraphicRect
{
public:
    /**
     * @brief PhGraphicTexturedRect
     * @param x
     * @param y
     * @param z
     * @param w
     * @param h
     * @param color
     * @param tv
     * @param tu
     * Constructor using PhGraphicRect::PhGraphicRect()
     */
    PhGraphicTexturedRect(int x, int y, int z, int w, int h, PhColor color, int tv = 1, int tu = 1);



    /**
     * @brief draw
     * @param px
     * draw the textured rectangle
     */
    void draw(int px);
protected:

    /**
     * @brief createTextureFromSurface
     * @param surface
     * create an OpenGL texture form a SDL Surface
     */
    void createTextureFromSurface(SDL_Surface * surface);
//    void setTexture(GLuint texture);

//    SDL_Surface * getSurface();
    /**
     * @brief getTexture
     * @return _texture
     * get the texture address(?)
     */
    GLuint getTexture();

private:
    /**
     * @brief createTextureFromColor
     * @param color
     */
    void createTextureFromColor(PhColor color);

    /**
     * @brief _texture
     * The texture address(?)
     */
    GLuint _texture;
    /**
     * @brief _tv
     * number of horizontal repetition
     */
    int _tv;
    /**
     * @brief _tu
     * number of vertical repetition
     */
    int _tu;
//  SDL_Surface *_surface;
};

#endif // PHGRAPHICTEXTUREDSQUARE_H
