/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include "PhColor.h"


PhColor::PhColor(PhString color) : QColor(color)
{
}

PhColor::PhColor(int r, int g, int b, int a) : QColor(r, g, b, a)
{
}

PhColor::PhColor() : QColor(255,255,255,255)
{
}

SDL_Color PhColor::toSDL()
{
    SDL_Color color = {this->red(), this->green(), this->blue(), this->alpha()};
    return color;
}
