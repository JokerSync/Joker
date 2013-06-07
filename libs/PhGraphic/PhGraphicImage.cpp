/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicImage.h"

PhGraphicImage::PhGraphicImage(PhString filename, int x, int y, int z, int w, int h, PhColor color, int tv, int tu) :
    PhGraphicTexturedRect(x, y, z, w, h, color, tv, tu)
{
    //this->setFilename(filename);
    SDL_Surface * surface = IMG_Load(filename.toStdString().c_str());
    createTextureFromSurface(surface);
    //SDL_FreeSurface(surface);
}

void PhGraphicImage::init()
{
}
void PhGraphicImage::dispose()
{
}

void PhGraphicImage::draw(int px)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
    PhGraphicTexturedRect::draw(px);
}


//void PhGraphicImage::setFilename(Phstring filename){
//    _filename = filename;
//}
//PhString PhGraphicImage::getFilename(){
//    return _filename;
//}


