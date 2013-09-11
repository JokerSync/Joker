/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICTEXT_H
#define PHGRAPHICTEXT_H

#include "PhGraphicTexturedRect.h"
#include "PhGraphicObject.h"

class PhGraphicText : public PhGraphicTexturedRect {
public:
//    GLuint createTextureFromSurface(SDL_Surface * surface);

	PhGraphicText();
	PhGraphicText(PhFont* font, QString content, int x = 0, int y = 0, int z = 0, int w = 0, int h = 0);
    /**
     * @brief Draw the text on screen
     *
     */
	void draw();

	bool init();

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
     * @brief getContent
     * @return _content
     */
	QString getContent();
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
