/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QtGlobal>

#if defined(Q_OS_WIN)
#include <GL/glu.h>
#else
#include <glu.h>
#endif

#include "PhFont.h"
#include "PhTools/PhDebug.h"

PhFont::PhFont() : _texture(-1), _glyphHeight(0), _boldness(0), _ready(false)
{
}

void PhFont::setFontFile(QString fontFile)
{
	if(fontFile != this->_fontFile) {
		PHDEBUG << fontFile;
		this->_fontFile = fontFile;
		_ready = false;
	}
}

QString PhFont::getFontFile()
{
	return _fontFile;
}

int PhFont::computeMaxFontSize(QString fileName)
{
	int size = 25;
	int fontHeight = 128;
	int low = 0, high = 1000;
	while (low < high) {
		size = (low + high) / 2;
		TTF_Font * font = TTF_OpenFont(fileName.toStdString().c_str(), size);
		//Break in case of issue with the file
		if(!font)
			return -1;

		if (fontHeight == TTF_FontHeight(font))
			break;
		else if (fontHeight < TTF_FontHeight(font))
			high = size - 1;
		else
			low = size + 1;
		TTF_CloseFont(font);
	}
	TTF_Font * font = TTF_OpenFont(fileName.toStdString().c_str(), size);
	if(fontHeight < TTF_FontHeight(font))
		size--;
	TTF_CloseFont(font);

	return size;
}

// This will split the setting of the bolness and the fontfile, which allow to change the boldness without reloading a font
bool PhFont::init()
{
	int size = computeMaxFontSize(_fontFile);

	if(size < 0)
		return false;
	PHDEBUG << "Opening" << _fontFile << "at size" << size;
	TTF_Font * font = TTF_OpenFont(_fontFile.toStdString().c_str(), size);

	//Font foreground color is white
	SDL_Color color = {255, 255, 255, 255};

	// used to set the base surface
	Uint32 rmask = 0x000000ff;
	Uint32 gmask = 0x0000ff00;
	Uint32 bmask = 0x00ff0000;
	Uint32 amask = 0xff000000;
	SDL_Surface * matrixSurface = SDL_CreateRGBSurface(0, 2048, 2048, 32, rmask, gmask, bmask, amask);

	// Font background color is transparent
	Uint32 backgroundColor = 0x00000000;
	SDL_FillRect(matrixSurface, NULL, backgroundColor);

	// Space between glyph
	int space = 128;
	_glyphHeight = 0;

	for(Uint16 ch = 0; ch < 32; ++ch) {
		_glyphAdvance[ch] = 0;
	}

	//set the boldness
	PHDEBUG << "Setting the font boldness to :" << _boldness;
	for(int boldIndex = 0; boldIndex <= _boldness; boldIndex++) {
		TTF_SetFontOutline(font, boldIndex);
		// We get rid of the 32 first useless char
		for(Uint16 charIndex = 32; charIndex < 256; ++charIndex) {
			if(TTF_GlyphIsProvided(font, charIndex) or charIndex == 153) {
				int minx, maxx, miny, maxy, advance;
				Uint16 charCode = charIndex;
				if(charCode == 153) {
					charCode = 339;
				}
				TTF_GlyphMetrics(font, charCode, &minx, &maxx, &miny, &maxy, &advance);
				if(advance > 0) {
					// First render the glyph to a surface
					SDL_Surface * glyphSurface = TTF_RenderGlyph_Blended(font, charCode, color);
					if (glyphSurface) {
						SDL_Rect glyphRect;
						glyphRect.x = (charIndex % 16) * space;
						glyphRect.y = (charIndex / 16) * space;
						glyphRect.w = glyphSurface->w;
						glyphRect.h = glyphSurface->h;
						if(glyphRect.h > _glyphHeight)
							_glyphHeight = glyphRect.h;
						//PHDEBUG << ch << (char) ch << minx << maxx << miny << maxy << advance << _glyphHeight;
						// Then blit it to the matrix
						SDL_BlitSurface( glyphSurface, NULL, matrixSurface, &glyphRect );

						// Store information about the glyph
						_glyphAdvance[charIndex] = advance;
					}
					else {
						_glyphAdvance[charIndex] = 0;
						PHDEBUG << "Error during the Render Glyph of " << (char) charIndex << SDL_GetError();
					}
					SDL_FreeSurface(glyphSurface);
				}
				else {
					PHDEBUG <<" Error with Glyph of char:" << charIndex << (char) charIndex << minx << maxx << miny << maxy << advance;
					_glyphAdvance[charIndex] = 0;
				}
			}
			else
				_glyphAdvance[charIndex] = 0;
		}
	}

	glEnable( GL_TEXTURE_2D );
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &_texture );

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, _texture );


	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, matrixSurface->format->BytesPerPixel, matrixSurface->w, matrixSurface->h, 0,
	              GL_RGBA, GL_UNSIGNED_BYTE, matrixSurface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Once the texture is created, the surface is no longer needed.
	SDL_FreeSurface(matrixSurface);
	TTF_CloseFont(font);

	_ready = true;
	return _ready;
}

int PhFont::getAdvance(unsigned char ch)
{
	return _glyphAdvance[ch];
}

void PhFont::select()
{
	if(!_ready)
		this->init();
	glBindTexture(GL_TEXTURE_2D, (GLuint)_texture);
}

int PhFont::getBoldness() const
{
	return _boldness;
}

int PhFont::getNominalWidth(QString string)
{
	int width = 0;
	foreach(QChar c, string) {
		width += getAdvance(c.toLatin1());
	}
	return width;
}

void PhFont::setBoldness(int value)
{
	if(_boldness != value) {
		_boldness = value;
		_ready = false;
	}
}

