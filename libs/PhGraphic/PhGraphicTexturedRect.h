/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICTEXTUREDSQUARE_H
#define PHGRAPHICTEXTUREDSQUARE_H

#include <QDebug>

#include <glu.h>

#include "PhGraphicRect.h"

class PhGraphicTexturedRect : public PhGraphicRect
{
public:

	PhGraphicTexturedRect(int x = 0, int y =0, int w = 0, int h = 0, int z = 0, float tu = 1, float tv = 1, QColor *color = new QColor());

    /**
     * @brief draw
     * @param px
     * draw the textured rectangle
     */
	void draw();

	void setTextureCoordinate(float tu, float tv);
protected:

    /**
     * @brief createTextureFromSurface
     * @param surface
     * create an OpenGL texture form a SDL Surface
     */
	bool createTextureFromSurface(SDL_Surface * surface);

    void createTextureFromColor(QColor color);
//    void setTexture(GLuint texture);

//    SDL_Surface * getSurface();
    /**
     * @brief getTexture
     * @return _texture
     * get the texture address(?)
     */
    GLuint getTexture();



private:

    /**
     * @brief _texture
     * The texture address(?)
     */
    GLuint _texture;

	/**
     * @brief _tu
	 * number of horizontal repetition
     */
	float _tu;

	/**
     * @brief _tv
	 * number of vertical repetition
     */
	float _tv;
};

#endif // PHGRAPHICTEXTUREDSQUARE_H
