/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICTEXTUREDSQUARE_H
#define PHGRAPHICTEXTUREDSQUARE_H

#include <QDebug>

#include <glu.h>

#include "PhTools/PhString.h"

#include "PhGraphicRect.h"

class PhGraphicTexturedRect : public PhGraphicRect
{
public:

	PhGraphicTexturedRect(int x = 0, int y =0, int w = 0, int h = 0, int z = 0, int tu = 1, int tv = 1, PhColor *color = new PhColor());

    /**
     * @brief draw
     * @param px
     * draw the textured rectangle
     */
	void draw();
protected:

    /**
     * @brief createTextureFromSurface
     * @param surface
     * create an OpenGL texture form a SDL Surface
     */
	bool createTextureFromSurface(SDL_Surface * surface);

    void createTextureFromColor(PhColor color);
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
     * @brief _texture
     * The texture address(?)
     */
    GLuint _texture;
    /**
     * @brief _tv
     * number of horizontal repetition
     */
	float _tv;
    /**
     * @brief _tu
     * number of vertical repetition
     */
	float _tu;
//  SDL_Surface *_surface;
};

#endif // PHGRAPHICTEXTUREDSQUARE_H
