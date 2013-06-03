/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHFONT_H
#define PHFONT_H

#include "SDL_ttf/SDL_ttf.h"

#include "PhString.h"

class PhFont
{
public:
    PhFont(PhString filename, int size);
    TTF_Font * getFont();
    PhString getFontName();

private:
    TTF_Font * _font;
    PhString _filename;
    int _size;
};

#endif // PHFONT_H
