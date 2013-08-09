/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicText.h"


PhGraphicText::PhGraphicText()
{
}

PhGraphicText::PhGraphicText(PhFont* font, QString content, int x, int y, int z, int w, int h, PhColor color) : _font(font), _content(content),
	PhGraphicTexturedRect(x, y , z, w, h), _color(color)
{
}

bool PhGraphicText::init()
{
	  SDL_Surface *surface = TTF_RenderUTF8_Blended(_font->getFont(),
												  _content.toStdString().c_str(),
												 _color.toSDL());

	qDebug() << "PhGraphicText::init";
    if(surface != NULL)
        this->createTextureFromSurface(surface);
    SDL_FreeSurface(surface);
	return true;
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

