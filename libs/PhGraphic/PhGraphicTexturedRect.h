/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICTEXTUREDSQUARE_H
#define PHGRAPHICTEXTUREDSQUARE_H

#include "PhGraphicRect.h"

/*!
 * \brief Draw a tetragon filed with an OpenGL Texture
 */
class PhGraphicTexturedRect : public PhGraphicRect
{
public:
	/*!
	 * \brief PhGraphicTexturedRect constructor
	 * \param x Upper left corner coordinate
	 * \param y Upper left corner coordinate
	 * \param w Desired width
	 * \param h Desired heigth
	 */
	PhGraphicTexturedRect(int x = 0, int y = 0, int w = 0, int h = 0);
	~PhGraphicTexturedRect();

	/**
	 * @brief draw
	 * draw the textured rectangle
	 */
	void draw();

	/*!
	 * \brief setTextureCoordinate
	 * \param tu
	 * \param tv
	 */
	void setTextureCoordinate(float tu, float tv);

	/*!
	 * \brief Create a texture from a RGB Buffer
	 * \param data the source buffer
	 * \param width the dimensions
	 * \param height the dimensions
	 * \return True if succeed, false otherwise
	 */
	bool createTextureFromARGBBuffer(void *data, int width, int height);

	/*!
	 * \brief Create a texture from a YUV Buffer
	 * \param data the source buffer
	 * \param width the dimensions
	 * \param height the dimensions
	 * \return True if succeed, false otherwise
	 */
	bool createTextureFromYUVBuffer(void *data, int width, int height);
protected:

	/**
	 * @brief createTextureFromSurface
	 * @param surface
	 * create an OpenGL texture form a SDL Surface
	 */
	bool createTextureFromSurface(SDL_Surface * surface);

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
