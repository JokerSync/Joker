/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <qmath.h>
#include <QColor>

#include "PhDebug.h"

#include "PhPictureTools.h"

void PhPictureTools::RGBtoYUV(const unsigned char *rgb, int *yuv, int monochrome, int luminance)
{
	if (monochrome) {
#if 1                           /* these are the two formulas that I found on the FourCC site... */
		yuv[0] = (int)(0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2]);
		yuv[1] = 128;
		yuv[2] = 128;
#else
		yuv[0] = (int)(0.257 * rgb[0]) + (0.504 * rgb[1]) + (0.098 * rgb[2]) + 16;
		yuv[1] = 128;
		yuv[2] = 128;
#endif
	}
	else {
#if 1                           /* these are the two formulas that I found on the FourCC site... */
		yuv[0] = (int)(0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2]);
		yuv[1] = (int)((rgb[2] - yuv[0]) * 0.565 + 128);
		yuv[2] = (int)((rgb[0] - yuv[0]) * 0.713 + 128);
#else
		yuv[0] = (0.257 * rgb[0]) + (0.504 * rgb[1]) + (0.098 * rgb[2]) + 16;
		yuv[1] = 128 - (0.148 * rgb[0]) - (0.291 * rgb[1]) + (0.439 * rgb[2]);
		yuv[2] = 128 + (0.439 * rgb[0]) - (0.368 * rgb[1]) - (0.071 * rgb[2]);
#endif
	}

	if (luminance != 100) {
		yuv[0] = yuv[0] * luminance / 100;
		if (yuv[0] > 255)
			yuv[0] = 255;
	}
}

void PhPictureTools::YUVtoRGB(const int *yuv, unsigned char *rgb)
{
	int r = (yuv[0] + 701 * (yuv[2] - 128) / 500);
	rgb[0] = (r > 255) ? 255 : (r < 0 ? 0 : (unsigned char)r);
	int g = (yuv[0] - (34414 * (yuv[1] - 128) + 71414 * (yuv[2] - 128)) / 100000);
	rgb[1] = (g > 255) ? 255 : (g < 0 ? 0 : (unsigned char)g);
	int b = (yuv[0] + 1772 * (yuv[1] - 128) / 1000);
	rgb[2] = (b > 255) ? 255 : (g < 0 ? 0 : (unsigned char)b);
}

void PhPictureTools::ConvertRGBtoYV12(const unsigned char *rgbIn, unsigned char *yuvOut, int w, int h, int monochrome, int luminance)
{
	unsigned char rgb[3];
	int yuv[3];
	unsigned char *op[3];

	op[0] = yuvOut;
	op[1] = op[0] + w*h;
	op[2] = op[1] + w*h/4;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			/* Swap R and B */
			rgb[0] = rgbIn[2];
			rgb[1] = rgbIn[1];
			rgb[2] = rgbIn[0];

			RGBtoYUV(rgb, yuv, monochrome, luminance);
			*(op[0]++) = yuv[0];
			if (x % 2 == 0 && y % 2 == 0) {
				*(op[1]++) = yuv[2];
				*(op[2]++) = yuv[1];
			}
			rgbIn += 3;
		}
	}
}

void PhPictureTools::ConvertYV12toRGB(const unsigned char *yuvIn, unsigned char *rgbOut, int w, int h)
{
	int yuv[3];
	const unsigned char *op[3];

	op[0] = yuvIn;
	op[1] = op[0] + w * h;
	op[2] = op[1] + w * h /4;
	for(int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			yuv[0] = *(op[0]++);
			if (x % 2 == 0 && y % 2 == 0) {
				yuv[2] = *(op[1]++);
				yuv[1] = *(op[2]++);
			}
			YUVtoRGB(yuv, rgbOut);
			rgbOut += 3;
		}
	}
}

unsigned char *PhPictureTools::generateRGBPattern(int w, int h)
{
	unsigned char * rgbOut = new unsigned char[w * h * 3];

	for(int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			rgbOut[3 * (y * w + x)] = 255 * y / h;
			rgbOut[3 * (y * w + x) + 1] = 255 * x / w;
			rgbOut[3 * (y * w + x) + 2] = 255 * x * y / w / h;
		}
	}
	return rgbOut;
}

unsigned char *PhPictureTools::generateYUVPattern(int w, int h)
{
	unsigned char * yuvOut = new unsigned char[w * h * 3 / 2];

	unsigned char *op[3];

	op[0] = yuvOut;
	op[1] = op[0] + w*h;
	op[2] = op[1] + w*h/4;

	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			*(op[0]++) = y * 256 / h;
			if (x % 2 == 0 && y % 2 == 0) {
				*(op[1]++) = 160;
				*(op[2]++) = 160;
			}
		}
	}

	return yuvOut;
}

int PhPictureTools::compare(QImage imageA, QImage imageB, bool log)
{
	int max = std::numeric_limits<int>::max();
	if(imageA.size() != imageB.size()) {
		PHDEBUG << "Size is different:" << imageA.size() << imageB.size();
		return max;
	}

	int result = 0;
	int w = imageA.width();
	int h = imageA.height();

	for(int i = 0; i < w; i++) {
		for(int j = 0; j < h; j++) {
			QRgb a = imageA.pixel(i, j);
			QRgb b = imageB.pixel(i, j);
			int diff = qPow(qRed(a) - qRed(b), 2) + qPow(qGreen(a) - qGreen(b), 2) + qPow(qBlue(a) - qBlue(b), 2);
			if(log && diff)
				PHDEBUG << QString("(%1, %2) %3 / %4 => %5 / %6")
				    .arg(i)
				    .arg(j)
				    .arg(QColor(a).name())
				    .arg(QColor(b).name())
				    .arg(diff)
				    .arg(result);
			result += diff;
			if(result > max / 2) {
				PHDEBUG << QString("(%1, %2) Too many difference detected: %3 / %4")
				    .arg(i)
				    .arg(j)
				    .arg(result)
				    .arg(max);
				return max;
			}
		}
	}

	return result;
}
