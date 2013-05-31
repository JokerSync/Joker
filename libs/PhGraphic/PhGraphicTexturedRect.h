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
    PhGraphicTexturedRect(int x, int y, int z, int w, int h, PhColor color, int tv = 1, int tu = 1);


    void draw();
protected:

    void createTextureFromSurface(SDL_Surface * surface);
//    void setTexture(GLuint texture);

//    SDL_Surface * getSurface();
    GLuint getTexture();

private:
    GLuint _texture;
    int _tv;
    int _tu;
//  SDL_Surface *_surface;
};

#endif // PHGRAPHICTEXTUREDSQUARE_H
