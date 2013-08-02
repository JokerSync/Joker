/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicText.h"


bool PhGraphicText::init()
{
	  SDL_Surface *surface = TTF_RenderUTF8_Blended(_font->getFont(),
												  _content.toStdString().c_str(),
												 _color.toSDL());
    if(surface != NULL)
        this->createTextureFromSurface(surface);
    SDL_FreeSurface(surface);
}

PhColor PhGraphicText::getColor(){
	return _color;
}

void PhGraphicText::setColor(PhColor color){
	_color = color;
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

void PhGraphicText::draw(int px)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    PhGraphicTexturedRect::draw(px);
}
void PhGraphicText::dispose()
{
    qDebug() << "Hi, this is dispose";
}

