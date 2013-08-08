/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHFONT_H
#define PHFONT_H

#if defined(Q_OS_MAC)
#include <SDL_ttf/SDL_ttf.h>
#else
#include <SDL/SDL_ttf.h>
#endif


#include "PhString.h"

class PhFont
{
public:
    /**
     * @brief PhFont
     * @param filename
     * @param size
     */
    PhFont(PhString filename, int size);
    /**
     * @brief getFont
     * @return _font
     */
    TTF_Font * getFont();
    /**
     * @brief getFontName
     * @return _filename
     */
    PhString getFontName();

	bool init();
private:
    /**
     * @brief _font
     */
    TTF_Font * _font;
    /**
     * @brief _filename
     */
    PhString _filename;
    /**
     * @brief _size
     */
    int _size;
};

#endif // PHFONT_H
