/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QStringList>

#include "PhFont.h"

PhFont::PhFont(QString filename, int size)
{
	_filename = filename;
	_size = size;
}

TTF_Font * PhFont::getFont()
{
	if (_font == NULL)
		init();
	return _font;
}

QString PhFont::getFontName(){
	QStringList list ;
	list = _filename.split("/");
	QString name = list.last().split(".").first();
	return name;
}

int PhFont::getAdvance(int ch)
{
	if (0 <= ch and ch < 256)
		return _glyphAdvance[ch];

	PHDEBUG << "The" << ch << "code is not an ASCII character";
	return 0;
}

int PhFont::getWidth(int ch)
{
	if (0 <= ch and ch < 256)
		return _glyphWidth[ch];

	PHDEBUG << "The" << ch << "code is not an ASCII character";
	return 0;
}


bool PhFont::init()
{
	_font = TTF_OpenFont(_filename.toStdString().c_str(), _size);

	//Font's color (black)
	SDL_Color color={0,0,0};
	Uint16 ch;


	// used to set the base surface
	Uint32 rmask, gmask, bmask, amask;
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
	_glyphMatrix = SDL_CreateRGBSurface(0,2048,2048,32,rmask,gmask,bmask,amask);

	Uint32 bgColor;
	bgColor = 0xffffffff;
	PHDEBUG << SDL_FillRect(_glyphMatrix, NULL, bgColor);

	// Space between glyph
	int space = 128;
	_glyphHeight = 0;

	// We get rid of the 32 first useless char
	for(ch = 32; ch < 256; ++ch)
	{
		if(TTF_GlyphIsProvided(_font, ch))
		{
			int minx, maxx, miny, maxy, advance;
			TTF_GlyphMetrics(_font, ch, &minx,&maxx, &miny, &maxy, &advance);
			//PHDEBUG << ch << (char) ch << minx << maxx << miny << maxy << advance;
			if(advance != 0)
			{
				// Temporary surface of the character
				SDL_Surface * glyphSurface = TTF_RenderGlyph_Blended(_font, ch, color);
				if (!glyphSurface)
					PHDEBUG << "Error during the Render Glyph of " << (char) ch << SDL_GetError();
				SDL_Rect glyphRect;
				glyphRect.x = (ch % 16) * space;
				glyphRect.y = (ch / 16) * space;
				glyphRect.w = glyphSurface->w;
				glyphRect.h = glyphSurface->h;
				if(glyphRect.h > _glyphHeight)
					_glyphHeight = glyphRect.h;
				if(SDL_BlitSurface( glyphSurface, NULL, _glyphMatrix, &glyphRect ))
					PHDEBUG << "Error during the blit of the glyph of" << (char) ch << SDL_GetError();

				// Store information about the glyph
				_glyphAdvance[ch] = advance;
				_glyphWidth[ch] = maxx - minx;
			}
			else
				PHDEBUG <<" Error with : " << ch << (char) ch << minx << maxx << miny << maxy << advance;
		}
		else{
			_glyphAdvance[ch] = 0;
			_glyphWidth[ch] = 0;
		}
	}

	// This is possible without switch because we manually set it up
	GLenum textureFormat;
	textureFormat = GL_RGBA;



	glEnable( GL_TEXTURE_2D );
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &_texture );

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, _texture );


	// Edit the texture object's image data using the information SDL_Surface gives us
	glTexImage2D( GL_TEXTURE_2D, 0, _glyphMatrix->format->BytesPerPixel, _glyphMatrix->w, _glyphMatrix->h, 0,
				  textureFormat, GL_UNSIGNED_BYTE, _glyphMatrix->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);



	return(_font != NULL);
}
