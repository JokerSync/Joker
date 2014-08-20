/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHPICTURETOOLS_H
#define PHPICTURETOOLS_H

#include <QImage>

/**
 * @brief It provides a lot of useful picture tools
 *
 * It can do space color convertions and picture pattern generation.
 */
class PhPictureTools
{
public:
	/**
	 * @brief Convert from RGB to YUV
	 * @param rgb pointer to the source frame
	 * @param yuv pointer to the destination frame
	 * @param monochrome desired monochrome value
	 * @param luminance desired luminance value
	 */
	static void RGBtoYUV(const unsigned char *rgb, int *yuv, int monochrome, int luminance);
	/**
	 * @brief Convert from YUV to RGB
	 * @param yuv pointer to the source frame
	 * @param rgb pointer to the destination frame
	 */
	static void YUVtoRGB(const int *yuv, unsigned char *rgb);
	/**
	 * @brief Convert from RGB to YV12
	 * @param rgbIn pointer to the source frame
	 * @param yuvOut pointer to the destination frame
	 * @param w source width
	 * @param h source height
	 * @param monochrome desired monochrome value
	 * @param luminance desired luminance value
	 */
	static void ConvertRGBtoYV12(const unsigned char *rgbIn, unsigned char *yuvOut, int w, int h, int monochrome, int luminance);
	/**
	 * @brief Convert from YV12 to RGB
	 * @param yuvIn pointer to the source frame
	 * @param rgbOut pointer to the destination frame
	 * @param w source width
	 * @param h source height
	 */
	static void ConvertYV12toRGB(const unsigned char *yuvIn, unsigned char *rgbOut, int w, int h);
	/**
	 * @brief generate a RGB pattern
	 *
	 * The pattern correspon to a red frame.
	 *
	 * @param w desired width
	 * @param h desired height
	 * @return a pointer to the RGB frame
	 */
	static unsigned char  * generateRGBPattern(int w, int h);
	/**
	 * @brief generate a YUV pattern
	 * @param w desired width
	 * @param h desired height
	 * @return a pointer to the RGB frame
	 */
	static unsigned char * generateYUVPattern(int w, int h);

	/**
	 * @brief Compare two QImage
	 * @param imageA An image
	 * @param imageB Another image
	 * @param log Log the difference
	 * @return The sum of the squared difference of the pixel RGB values.
	 */
	static int compare(QImage imageA, QImage imageB, bool log = false);
};

#endif // PHPICTURETOOLS_H
