/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHGRAPHICIMAGE_H
#define PHGRAPHICIMAGE_H

#include "PhGraphicTexturedRect.h"

/**
 * @brief Provide a way to display the images using OpenGL
 */
class PhGraphicImage : public PhGraphicTexturedRect
{
public:

	/**
	 * @brief PhGraphicImage constructor
	 * @param filename the source image url
	 * @param x Upper left corner coordinates of the display rect
	 * @param y Upper left corner coordinates of the display rect
	 * @param w Desired width
	 * @param h Desired height
	 */
	PhGraphicImage( QString filename = "", int x = 0, int y = 0, int w = 0, int h = 0);

	/**
	 * @brief Set the file name
	 * @param filename
	 */
	void setFilename(QString filename);
	/**
	 * @brief Get the file name
	 * @return the file name
	 */
	QString fileName();

	/**
	 * @brief draw
	 * draw the PhGraphicImage using PhGraphicTexturedRect::draw()
	 */
	void draw();

	/**
	 * @brief originalSize
	 * @return the original image size
	 */
	QSize originalSize() const;

protected:
	/**
	 * @brief Initialize the image texture
	 */
	bool init();

	/**
	 * @brief Dispose the image texture
	 */
	void dispose();

private:

	QString _filename;

	SDL_Surface * _surface;
	QSize _originalSize;
};

#endif // PHGRAPHICIMAGE_H
