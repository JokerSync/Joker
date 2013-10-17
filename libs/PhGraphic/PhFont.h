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

#include <glu.h>

#include "PhTools/PhDebug.h"

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

	int getAdvance (int ch);
	int getWidth (int ch);
	int getSpace(){return _space;}
	int getHeight(){return _glyphHeight;}
	void select();
	SDL_Surface * getMatrixSurface(){return _glyphMatrix;}
	GLuint getMatrixTexture(){return _texture;}

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

	int _space = 128;

	SDL_Surface * _glyphMatrix;
	/**
	 * @brief _texture
	 * The texture address(?)
	 */
	GLuint _texture;


	// store the width of each glyph
	int _glyphWidth[256];
	int _glyphAdvance[256];
	int _glyphHeight;
};

#endif // PHFONT_H
