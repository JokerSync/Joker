/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICTEXTUREDSQUARE_H
#define PHGRAPHICTEXTUREDSQUARE_H

#include "PhGraphicRect.h"

/**
 * @brief Draw a tetragon filed with an OpenGL Texture
 */
class PhGraphicTexturedRect : public PhGraphicRect
{
public:
	/**
	 * @brief PhGraphicTexturedRect constructor
	 * @param x Upper left corner coordinate
	 * @param y Upper left corner coordinate
	 * @param w Desired width
	 * @param h Desired heigth
	 */
	PhGraphicTexturedRect(int x = 0, int y = 0, int w = 0, int h = 0);
	~PhGraphicTexturedRect();

	/**
	 * @brief draw
	 * draw the textured rectangle
	 */
	void draw();

	/**
	 * @brief setTextureCoordinate
	 * @param tu
	 * @param tv
	 */
	void setTextureCoordinate(float tu, float tv);

	/**
	 * @brief Create a texture from a RGBA Buffer
	 * @param data the source buffer
	 * @param width the dimensions
	 * @param height the dimensions
	 * @return True if succeed, false otherwise
	 */
	bool createTextureFromARGBBuffer(void *data, int width, int height);

	/**
	 * @brief Create a texture from a RGBA Buffer
	 * @param data the source buffer
	 * @param width the dimensions
	 * @param height the dimensions
	 * @return True if succeed, false otherwise
	 */
	bool createTextureFromBGRABuffer(void *data, int width, int height);

	/**
	 * @brief Create a texture from a RGB Buffer
	 * @param data the source buffer
	 * @param width the dimensions
	 * @param height the dimensions
	 * @return True if succeed, false otherwise
	 */
	bool createTextureFromRGBBuffer(void *data, int width, int height);

	/**
	 * @brief Create a texture from a YUV Buffer
	 * @param data the source buffer
	 * @param width the dimensions
	 * @param height the dimensions
	 * @return True if succeed, false otherwise
	 */
	bool createTextureFromYUVBuffer(void *data, int width, int height);

	/**
	 * @brief Enable or disable the texture repetition
	 * Texture repetition is disabled by default.
	 * @param repeat True to enable repetition
	 */
	void setRepeat(bool repeat);

	/**
	 * @brief Retrieve the texture repetition
	 * @return True if repetition is enabled
	 */
	bool getRepeat();

	/**
	 * @brief Enable or disable the texture bilinear filtering
	 * Texture bilinear filtering is enabled by default.
	 * @param bilinear True to enable bilinear filtering
	 */
	void setBilinearFiltering(bool bilinear);

	/**
	 * @brief Retrieve the texture filtering
	 * @return True if bilinear filtering is enabled
	 */
	bool getBilinearFiltering();

protected:

	/**
	 * @brief createTextureFromSurface
	 * @param surface
	 * create an OpenGL texture form a SDL Surface
	 */
	bool createTextureFromSurface(SDL_Surface * surface);

	/**
	 * @brief initTextures
	 * initialize the textures objects if then do not exist yet
	 * @return True if succeed, false otherwise
	 */
	bool initTextures();

	/**
	 * @brief swapTextures
	 * swap the current and previous textures to achieve double-buffering and avoid waiting
	 * for the OpenGL driver to finish rendering
	 */
	void swapTextures();

	/**
	 * @brief applyTextureSettings
	 * Applies the texture rendering settings
	 */
	void applyTextureSettings();

private:

	/**
	 * @brief _currentTexture
	 * The texture address(?)
	 */
	GLuint _currentTexture;

	/**
	 * @brief _previousTexture
	 * The texture address(?)
	 */
	GLuint _previousTexture;

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

	int _textureWidth;
	int _textureHeight;

	/**
	 * @brief _repeat
	 * Whether the texture should be repeated.
	 * Texture repetition is disabled by default.
	 */
	bool _repeat;

	/**
	 * @brief _bilinearFiltering
	 * Whether the texture should be scaled with bilinear filtering
	 * or nearest neighbor.
	 * Texture bilinear filtering is enabled by default.
	 */
	bool _bilinearFiltering;
};

#endif // PHGRAPHICTEXTUREDSQUARE_H
