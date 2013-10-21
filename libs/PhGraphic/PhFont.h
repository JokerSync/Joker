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
     */
	PhFont(QString filename);

	~PhFont();
    /**
     * @brief getFontName
     * @return _filename
     */
	QString getFontName();

	int getAdvance (int ch);
	int getHeight(){return _glyphHeight;}
	void select();
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
	 * @brief _texture
	 * The texture address(?)
	 */
	GLuint _texture;

	// store the width of each glyph
	int _glyphAdvance[256];
	int _glyphHeight;
};

#endif // PHFONT_H
