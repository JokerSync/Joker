/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhGraphicTexturedRect.h"

PhGraphicTexturedRect::PhGraphicTexturedRect(int x, int y, int w, int h)
	: PhGraphicRect(x, y, w, h),
	_currentTexture(0),
	_previousTexture(0),
	_tu(1.0f),
	_tv(1.0f),
	_textureWidth(0),
	_textureHeight(0),
	_repeat(false),
	_bilinearFiltering(true)
{

}

PhGraphicTexturedRect::~PhGraphicTexturedRect()
{
}

bool PhGraphicTexturedRect::initTextures() {
	// Have OpenGL generate a texture object handle for us
	if(_currentTexture == 0) {
		glGenTextures( 1, &_currentTexture );
		glGenTextures( 1, &_previousTexture );
		if(_currentTexture == 0 or _previousTexture == 0) {
			PHDEBUG << "glGenTextures() errored: is opengl context ready?";
			return false;
		}
	}
	return true;
}

/* Swap the current and previous textures to achieve double-buffering and avoid waiting
 * for the OpenGL driver to finish rendering.
 * More info here: https://developer.apple.com/library/mac/documentation/graphicsimaging/conceptual/opengl-macprogguide/opengl_texturedata/opengl_texturedata.html#//apple_ref/doc/uid/TP40001987-CH407-SW16
 */
void PhGraphicTexturedRect::swapTextures()
{
	GLuint tempTexture;

	tempTexture = _currentTexture;
	_currentTexture = _previousTexture;
	_previousTexture = tempTexture;
}

bool PhGraphicTexturedRect::createTextureFromSurface(SDL_Surface *surface)
{
	swapTextures();

	glEnable( GL_TEXTURE_2D );
	if(!initTextures()) {
		return false;
	}

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, _currentTexture );

	GLenum textureFormat = 0;

	switch (surface->format->BytesPerPixel) {
	case 1:
		textureFormat = GL_ALPHA;
		break;
	case 3: // no alpha channel
		if (surface->format->Rmask == 0x000000ff)
			textureFormat = GL_RGB;
		else
			textureFormat = GL_BGR;
		break;
	case 4: // contains an alpha channel
		if (surface->format->Rmask == 0x000000ff)
			textureFormat = GL_RGBA;
		else
			textureFormat = GL_BGRA;

		break;
	default:
		PHDEBUG << "Warning: the image is not truecolor...";
		return false;
	}

	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0,
	              textureFormat, GL_UNSIGNED_BYTE, surface->pixels);

	applyTextureSettings();

	return true;
}

bool PhGraphicTexturedRect::createTextureFromARGBBuffer(void *data, int width, int height)
{
	swapTextures();

	glEnable( GL_TEXTURE_2D );

	if(!initTextures()) {
		return false;
	}

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, _currentTexture );


	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
	              GL_RGBA, GL_UNSIGNED_BYTE, data);

	applyTextureSettings();

	return true;
}

bool PhGraphicTexturedRect::createTextureFromBGRABuffer(void *data, int width, int height)
{
	swapTextures();

	glEnable( GL_TEXTURE_2D );

	if(!initTextures()) {
		return false;
	}

	if((width != _textureWidth) || (height != _textureHeight)) {
		_textureWidth = width;
		_textureHeight = height;

		PHDEBUG << QString("%1x%2").arg(width).arg(height);

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, _previousTexture );

		// Edit the texture object's image data using the information SDL_Surface gives us
		// Note: Store internally in GL_RGBA8, and upload from our buffer which is
		// GL_BGRA/GL_UNSIGNED_INT_8_8_8_8_REV.
		// This combination is supposed to be optimal on Windows with both nVidia and AMD,
		// avoiding all	CPU-based conversion and allowing direct DMA to video card
		// See: http://www.opengl.org/wiki/Common_Mistakes#Slow_pixel_transfer_performance
		// Also see: http://www.opengl.org/wiki/Common_Mistakes#Image_precision
		// It is also optimal for MacOS. See 'Optimal Data Formats and Types' in:
		// https://developer.apple.com/library/mac/documentation/graphicsimaging/conceptual/opengl-macprogguide/opengl_texturedata/opengl_texturedata.html#//apple_ref/doc/uid/TP40001987-CH407-SW1
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		              GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data);

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, _currentTexture );

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
		              GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
	}
	else {
		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, _currentTexture );
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
	}

	applyTextureSettings();

	return true;
}

bool PhGraphicTexturedRect::createTextureFromRGBBuffer(void *data, int width, int height)
{
	swapTextures();

	glEnable( GL_TEXTURE_2D );

	if(!initTextures()) {
		return false;
	}

	if((width != _textureWidth) || (height != _textureHeight)) {
		_textureWidth = width;
		_textureHeight = height;

		PHDEBUG << QString("%1x%2").arg(width).arg(height);

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, _previousTexture );

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		              GL_RGB, GL_UNSIGNED_BYTE, data);

		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, _currentTexture );

		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
		              GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, _currentTexture );
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	applyTextureSettings();

	return true;
}


bool PhGraphicTexturedRect::createTextureFromYUVBuffer(void *data, int width, int height)
{
	swapTextures();

	glEnable( GL_TEXTURE_2D );

	if(!initTextures()) {
		return false;
	}

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, _currentTexture );

	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
	              GL_YCBCR_422_APPLE, GL_UNSIGNED_SHORT_8_8_APPLE, data);

	applyTextureSettings();

	return true;
}


void PhGraphicTexturedRect::draw()
{
	PhGraphicRect::draw();

	glBindTexture(GL_TEXTURE_2D, _currentTexture);

	glEnable(GL_TEXTURE_2D);

	//        (0,0) ------ (1,0)
	//          |            |
	//          |            |
	//        (0,1) ------ (1,1)

	glBegin(GL_QUADS);  //Begining the cube's drawing
	{
		glTexCoord3f(0, 0, 1);      glVertex3i(this->x(),      this->y(), this->z());
		glTexCoord3f(_tu, 0, 1);    glVertex3i(this->x() + this->width(), this->y(), this->z());
		glTexCoord3f(_tu, _tv, 1);  glVertex3i(this->x() + this->width(), this->y() + this->height(),  this->z());
		glTexCoord3f(0, _tv, 1);    glVertex3i(this->x(),      this->y() + this->height(),  this->z());
	}
	glEnd();


	glDisable(GL_TEXTURE_2D);
}

void PhGraphicTexturedRect::setTextureCoordinate(float tu, float tv)
{
	_tu = tu;
	_tv = tv;
}

void PhGraphicTexturedRect::setRepeat(bool repeat)
{
	_repeat = repeat;

	if (_currentTexture != 0 and _previousTexture != 0) {
		glBindTexture(GL_TEXTURE_2D, _currentTexture);
		applyTextureSettings();
		glBindTexture(GL_TEXTURE_2D, _previousTexture);
		applyTextureSettings();
	}
}

bool PhGraphicTexturedRect::getRepeat()
{
	return _repeat;
}

void PhGraphicTexturedRect::setBilinearFiltering(bool bilinear)
{
	_bilinearFiltering = bilinear;

	if (_currentTexture != 0 and _previousTexture != 0) {
		glBindTexture(GL_TEXTURE_2D, _currentTexture);
		applyTextureSettings();
		glBindTexture(GL_TEXTURE_2D, _previousTexture);
		applyTextureSettings();
	}
}

bool PhGraphicTexturedRect::getBilinearFiltering()
{
	return _bilinearFiltering;
}

void PhGraphicTexturedRect::applyTextureSettings()
{
	int filterSetting;
	if (_bilinearFiltering) {
		// Use bilinear filtering to scale the texture to the window size.
		// This produces smooth scaled images.
		filterSetting = GL_LINEAR;
	}
	else {
		// Use nearest-neighbour filtering to scale the texture to
		// the window size.
		// This produces scaled images that are 'blocky'.
		// This is used for the test suite.
		filterSetting = GL_NEAREST;
	}

	int wrapSetting;
	if (_repeat) {
		// render the texture as a repeating pattern
		wrapSetting = GL_REPEAT;
	}
	else {
		// Make sure the colors are ok at the edges of the texture,
		// by setting the wrapping to 'clamp' instead of 'repeat'
		wrapSetting = GL_CLAMP_TO_EDGE;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterSetting);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterSetting);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapSetting);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapSetting);
}
