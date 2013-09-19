/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicText.h"

PhGraphicText::PhGraphicText(PhFont* font, QString content, int x, int y, int w, int h)
	: PhGraphicTexturedRect(x, y, w, h), _font(font), _content(content)
{
}

bool PhGraphicText::init()
{
	SDL_Color color = {_color.red(), _color.green(), _color.blue(), _color.alpha() };

	SDL_Surface *surface = TTF_RenderUTF8_Blended(_font->getFont(),
												  _content.toStdString().c_str(),
												  color);

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
	glColor3f(_color.redF(), _color.greenF(), _color.blueF());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	PhGraphicTexturedRect::draw();

	glDisable(GL_BLEND);
}

