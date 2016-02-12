/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHFONT_H
#define PHFONT_H

#include <QString>

/**
 * @brief Describe the font appearance for PhGraphicText
 *
 * The PhFont instance are initialized with a true type font file.
 * The boldness can be configured.
 */
class PhFont
{
public:
	/**
	 * @brief PhFont constructor
	 */
	PhFont();

	/**
	 * @brief Check is the font is ready for drawing
	 * @return True if ready, false otherwise
	 */
	bool ready();

	/**
	 * @brief Set the source font file.
	 * @param fontFile Path to the new font file
	 * @return true if the font was loaded successfully, false otherwise.
	 */
	void setFontFile(QString fontFile);

	/**
	 * @brief Get the font file
	 * @return the path to the font file
	 */
	QString fontFile();

	/**
	 * @brief Get the regular advance offset of a character.
	 *
	 * The returned value is correspond to the amount of pixel the character at a regular text size (100).
	 * This value must be converted proportionaly if the text width is scaled.
	 * @param ch ASCII index of the character.
	 * @return A value in pixel.
	 */
	int getAdvance(unsigned char ch);

	/**
	 * @brief Get the regular height of the font.
	 *
	 * The returned value is correspond to the amount of pixel the character at a regular text size (100).
	 * This value must be converted proportionaly if the text width is scaled.
	 * @return A value in pixel.
	 */
	int height() {
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

	/**
	 * @brief Get the nominal width of a given string
	 * @param string to be measured
	 * @return The length
	 */
	int getNominalWidth(QString string);

	/**
	 * @brief Compute the maximum font size
	 * @param fileName A font file
	 * @param boldness The font expected boldness
	 *
	 * Compute the maximum font size to initialize the font with
	 * so the glyph (including the boldness effect) never take
	 * more than 128 pixel
	 *
	 * @return A font size.
	 */
	static int computeMaxFontSize(QString fileName, int boldness);
private:
	/**
	 * @brief _texture
	 * The texture reference
	 */
	unsigned int _texture;

	bool init();

	/**
	 * @brief Store the regular advance of each glyph.
	 */
	int _glyphAdvance[256];

	/**
	 * @brief Store the regular advance of the font.
	 */
	int _glyphHeight;

	QString _fontFile;

	int _boldness;

	bool _ready;
};

#endif // PHFONT_H
