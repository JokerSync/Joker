/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicText.h"


PhGraphicText::PhGraphicText(PhString content, int x, int y, int z, int w, int h, PhFont *font, PhColor color) :
    PhGraphicTexturedRect(x, y, z, w, h, color)
{
    this->setContent(content);
    this->setFont(font);

    SDL_Color textColor={ 230, 230, 0, 0 };
    SDL_Surface *surface = TTF_RenderUTF8_Blended(font->getFont(),
                                                  content.toStdString().c_str(),
                                                  textColor);
    if(surface != NULL)
        this->createTextureFromSurface(surface);
    SDL_FreeSurface(surface);
}

void PhGraphicText::setContent(PhString content){
    _content = content;
}
void PhGraphicText::setFont(PhFont * font){
    _font = font;
}

PhString PhGraphicText::getContent(){
    return _content;
}
PhFont * PhGraphicText::getFont(){
    return _font;
}

void PhGraphicText::draw()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    PhGraphicTexturedRect::draw();
}
void PhGraphicText::dispose()
{
    qDebug() << "Hi, this is dispose";
}


void PhGraphicText::init()
{
    qDebug() << "Hi, this is init";
}
