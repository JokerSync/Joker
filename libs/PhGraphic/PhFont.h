/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHFONT_H
#define PHFONT_H

#include <QString>
#include <SDL2/SDL_ttf.h>

/**
 * @brief The PhFont class
 *
 * It provides a lot of tools which allow the users to compute texts
 */
class PhFont
{
public:
	/**
	 * @brief PhFont constructor
	 */
	PhFont();

	/**
	 * @brief Set the source font file.
	 * @param fontFile Path to the new font file
	 * @return true if the font was loaded successfully, false otherwise.
	 */
	bool setFontFile(QString fontFile);

	/**
	 * @brief Get the font file
	 * @return the path to the font file
	 */
	QString getFontFile();

	/**
	 * @brief Get the regular advance offset of a character.
	 *
	 * The returned value is correspond to the amount of pixel the character at a regular text size (100).
	 * This value must be converted proportionaly if the text width is scaled.
	 * @param ch ASCII index of the character.
	 * @return A value in pixel.
	 */
	int getAdvance (unsigned char ch);

	/**
	 * @brief Get the regular height of the font.
	 *
	 * The returned value is correspond to the amount of pixel the character at a regular text size (100).
	 * This value must be converted proportionaly if the text width is scaled.
	 * @return A value in pixel.
	 */
	int getHeight() {
		return _glyphHeight;
	}

	/**
	 * @brief Select the font for the further rendering operation.
	 */
	void select();

	/**
	 * @brief Set the font boldness
	 * The boldness is created using <a href=http://www.libsdl.org/projects/SDL_ttf/docs/SDL_ttf_24.html#SEC24>TTF_SetFontOutline</a>
	 * multiple times from 0 to value
	 * @param value the number of pass
	 */
	void setBoldness(int value);

	/**
	 * @brief Get boldness
	 * @return the number of outline pass
	 */
	int getBoldness() const;

private:
	/**
	 * @brief _texture
	 * The texture reference
	 */
	unsigned int _texture;

	bool init(QString fontFile);

	/**
	 * @brief Store the regular advance of each glyph.
	 */
	int _glyphAdvance[256];

	/**
	 * @brief Store the regular advance of the font.
	 */
	int _glyphHeight;

	TTF_Font * font;

	QString fontFile;

	int boldness;
};

#endif // PHFONT_H
