/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICTEXT_H
#define PHGRAPHICTEXT_H

#include "PhGraphicRect.h"
#include "PhFont.h"

/**
 * @brief A graphic text object
 *
 * This graphic text object is specified with:
 * - a content
 * - a color, position and size (inherited from PhGraphicRect)
 * - a PhFont
 */
class PhGraphicText : public PhGraphicRect {
public:
	/**
	 * @brief PhGraphicText constructor
	 * @param font The font
	 * @param content The content
	 * @param x The x coordinate
	 * @param y The y coordinate
	 * @param w The width
	 * @param h The height
	 */
	PhGraphicText(PhFont* font, QString content = "", int x = 0, int y = 0, int w = 0, int h = 0);

	/**
	 * @brief Draw the text on screen
	 *
	 */
	void draw();

	/**
	 * @brief setContent
	 * @param content
	 * Set the PhGraphicText content
	 */
	void setContent(QString content);
	/**
	 * @brief setFont
	 * @param font
	 * Set the PhGraphicText font
	 */
	void setFont(PhFont * font);

	/**
	 * @brief get the text content
	 * @return A string
	 */
	QString content();
	/**
	 * @brief getFont
	 * @return _font
	 */
	PhFont * getFont();



private:
	/**
	 * @brief _font
	 */
	PhFont *_font;

	/**
	 * @brief _content
	 */
	QString _content;
};

#endif // PHGRAPHICTEXT_H
