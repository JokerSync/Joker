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
	 * @brief Set the source family.
	 * @param family Tthe new font family
	 * @return true if the font was loaded successfully, false otherwise.
	 */
	void setFamily(QString family);

	/**
	 * @brief Get the font family
	 * @return The font family
	 */
	QString family();

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
	 * @brief Set the font weight
	 * @param weight An int value
	 */
	void setWeight(int weight);

	/**
	 * @brief Get font weight
	 * @return An int value
	 */
	int weight() const;

	/**
	 * @brief Get the nominal width of a given string
	 * @param string to be measured
	 * @return The length
	 */
	int getNominalWidth(QString string);

	/**
	 * @brief Compute the maximum font size
	 * @param family A font family
	 *
	 * Compute the maximum font size to initialize the font with
	 * so the glyph never take more than 128 pixel
	 *
	 * @return A font size.
	 */
	static int computeMaxFontSize(QString family);
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

	QString _family;

	int _weight;

	bool _ready;
};

#endif // PHFONT_H
