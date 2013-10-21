/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QStringList>

#include "PhFont.h"

PhFont::PhFont(QString filename): _font(NULL), _texture(-1)
{
	_filename = filename;
}

PhFont::~PhFont()
{
	if(_font)
		TTF_CloseFont(_font);
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

bool PhFont::init()
{
	_font = TTF_OpenFont(_filename.toStdString().c_str(), 100);
	if(_font == NULL)
		return true;

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

	// We get rid of the 32 first useless char
	for(Uint16 ch = 0; ch < 256; ++ch)
	{
		if(TTF_GlyphIsProvided(_font, ch))
		{
			int minx, maxx, miny, maxy, advance;
			TTF_GlyphMetrics(_font, ch, &minx,&maxx, &miny, &maxy, &advance);
			if(advance != 0)
			{
				// First render the glyph to a surface
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
				PHDEBUG << ch << (char) ch << minx << maxx << miny << maxy << advance << _glyphHeight;
				// Then blit it to the matrix
				SDL_BlitSurface( glyphSurface, NULL, matrixSurface, &glyphRect );

				// Store information about the glyph
				_glyphAdvance[ch] = advance;

				SDL_FreeSurface(glyphSurface);
			}
			else
				PHDEBUG <<" Error with : " << ch << (char) ch << minx << maxx << miny << maxy << advance;
		}
		else
			_glyphAdvance[ch] = 0;
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

	return true;
}
