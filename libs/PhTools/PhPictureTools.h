#ifndef PHPICTURETOOLS_H
#define PHPICTURETOOLS_H

class PhPictureTools
{
public:
	static void RGBtoYUV(const unsigned char *rgb, int *yuv, int monochrome, int luminance);
	static void YUVtoRGB(const int *yuv, unsigned char *rgb);

	static void ConvertRGBtoYV12(const unsigned char *rgbIn, unsigned char *yuvOut, int w, int h, int monochrome, int luminance);
	static void ConvertYV12toRGB(const unsigned char *yuvIn, unsigned char *rgbOut, int w, int h);

	static unsigned char  * generateRGBPattern(int w, int h);
	static unsigned char * generateYUVPattern(int w, int h);
};

#endif // PHPICTURETOOLS_H
