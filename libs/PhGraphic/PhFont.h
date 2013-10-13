/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHFONT_H
#define PHFONT_H

#include <QtGlobal>
#include <QString>

#if defined(Q_OS_MAC)
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_ttf.h>
#endif

class PhFont
{
public:
    /**
     * @brief PhFont
     * @param filename
     * @param size
     */
	PhFont(QString filename, int size);
    /**
     * @brief getFont
     * @return _font
     */
    TTF_Font * getFont();
    /**
     * @brief getFontName
     * @return _filename
     */
	QString getFontName();

	bool init();
private:
    /**
     * @brief _font
     */
    TTF_Font * _font;
    /**
     * @brief _filename
     */
	QString _filename;
    /**
     * @brief _size
     */
    int _size;
};

#endif // PHFONT_H
