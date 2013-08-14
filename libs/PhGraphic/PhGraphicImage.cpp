/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicImage.h"

#if defined(Q_OS_MAC)
#include <SDL_image/SDL_image.h>
#else
#include <SDL/SDL_image.h>
#endif

PhGraphicImage::PhGraphicImage(QString filename, int x, int y, int w, int h, int z, float tu, float tv, PhColor *color)
	: PhGraphicTexturedRect(x, y, w, h, z, tu, tv, color), _filename(filename), _surface(NULL)
{
}

bool PhGraphicImage::init()
{
    _surface = IMG_Load(_filename.toStdString().c_str());
    if(_surface != NULL)
	{
		if(createTextureFromSurface(_surface))
		{
			qDebug() << "Loading image";
			return true;
		}
	}

	qDebug()<<"Error loading:"<<_filename;
	return false;

}
void PhGraphicImage::dispose()
{
    SDL_FreeSurface(_surface);
}

void PhGraphicImage::draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
	PhGraphicTexturedRect::draw();
}

void PhGraphicImage::setFilename(QString filename){
    _filename = filename;
}

QString PhGraphicImage::getFilename(){
    return _filename;
}
