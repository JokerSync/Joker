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
#include "SDL/SDL.h"

#include "PhTools/PhString.h"

GLuint createTextureFromSurface(SDL_Surface * surface);
GLuint createSurfaceFromText(PhString text);

#endif // PHGRAPHICTEXT_H
