/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicText.h"


PhGraphicText::PhGraphicText()
{
}

PhGraphicText::PhGraphicText(PhFont* font, QString content, int x, int y, int z, int w, int h, int tu, int tv, QColor *color)
	: _font(font), _content(content), PhGraphicTexturedRect(x, y , z, w, h, tu, tv, color)
{
}

bool PhGraphicText::init()
{
	SDL_Color color = {this->getColor()->red(), this->getColor()->green(), this->getColor()->blue(), this->getColor()->alpha()};

	  SDL_Surface *surface = TTF_RenderUTF8_Blended(_font->getFont(),
												  _content.toStdString().c_str(),
												 color);

	qDebug() << "PhGraphicText::init";
    if(surface != NULL)
        this->createTextureFromSurface(surface);
    SDL_FreeSurface(surface);
	return true;
}

void PhGraphicText::setContent(QString content){
    _content = content;
}
void PhGraphicText::setFont(PhFont * font){
    _font = font;
}

QString PhGraphicText::getContent(){
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

