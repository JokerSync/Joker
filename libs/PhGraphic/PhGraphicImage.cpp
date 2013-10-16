/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhTools/PhDebug.h"
#include "PhGraphicImage.h"

#if defined(Q_OS_MAC)
#include <SDL2_image/SDL_image.h>
#else
#include <SDL2/SDL_image.h>
#endif

PhGraphicImage::PhGraphicImage(QString filename, int x, int y, int w, int h)
	: PhGraphicTexturedRect(x, y, w, h), _filename(filename), _surface(NULL)
{
}

bool PhGraphicImage::init()
{
    _surface = IMG_Load(_filename.toStdString().c_str());
    if(_surface != NULL)
	{
		if(createTextureFromSurface(_surface))
		{
			PHDEBUG << "Loading image";
			return true;
		}
	}

	PHDEBUG<<"Error loading:"<< _filename;
	return false;

}

void PhGraphicImage::dispose()
{
    SDL_FreeSurface(_surface);
}

void PhGraphicImage::draw()
{
	glColor3f(1, 1, 1);
	PhGraphicTexturedRect::draw();
}

void PhGraphicImage::setFilename(QString filename){
    _filename = filename;
}

QString PhGraphicImage::getFilename(){
    return _filename;
}
