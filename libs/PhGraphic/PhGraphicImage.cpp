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


void PhGraphicImage::init()
{
    _surface = IMG_Load(_filename.toStdString().c_str());
    if(_surface != NULL)
	{
		createTextureFromSurface(_surface);
		qDebug() << "Loading image";
	}
    else
        qDebug()<<"Error loading:"<<_filename;

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

void PhGraphicImage::setFilename(PhString filename){
    _filename = filename;
}

PhString PhGraphicImage::getFilename(){
    return _filename;
}
