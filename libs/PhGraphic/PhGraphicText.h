/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICTEXT_H
#define PHGRAPHICTEXT_H

#include <QDebug>


#include "PhGraphicTexturedRect.h"
#include "PhString.h"
#include "PhGraphicObject.h"


class PhGraphicText : public PhGraphicTexturedRect {
public:
//    GLuint createTextureFromSurface(SDL_Surface * surface);
//    GLuint createSurfaceFromText(PhString text);

    /**
     * @brief Draw the text on screen
     *
     */
	void draw();
    /**
     * @brief init
     * currently unused
     */
	bool init();
    /**
     * @brief dispose
     * currently unused
     */
    void dispose();

    /**
     * @brief setContent
     * @param content
     * Set the PhGraphicText content
     */
    void setContent(PhString content);
    /**
     * @brief setFont
     * @param font
     * Set the PhGraphicText font
     */
    void setFont(PhFont * font);

    /**
     * @brief getContent
     * @return _content
     */
    PhString getContent();
    /**
     * @brief getFont
     * @return _font
     */
    PhFont * getFont();

	/**
	 * @brief setColor
	 * @param color
	 * Set the PhColor of the PhGraphicObject
	 */
	void setColor(PhColor color);

	/**
	 * @brief getColor
	 * @return
	 */
	PhColor getColor();

private:
    /**
     * @brief _content
     */
    PhString _content;
    /**
     * @brief _font
     */
    PhFont *_font;

	/**
	 * @brief _color
	 * The PhColor of the PhGraphicObject
	 */
	PhColor _color;

};

#endif // PHGRAPHICTEXT_H
