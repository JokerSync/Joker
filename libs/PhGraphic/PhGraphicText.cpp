/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicText.h"

PhGraphicText::PhGraphicText(PhFont* font, QString content, int x, int y, int w, int h)
	: PhGraphicRect(x, y, w, h), _font(font), _content(content)
{
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


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D, _font->getMatrixTexture());
	glEnable(GL_TEXTURE_2D);

	int widthContent = 0;
	//Compute the natural width of the content to scale it later
	for(int i = 0; i < _content.length(); i++)
	{
		widthContent += _font->getWidth((int)_content.at(i).toLatin1());
	}


	int x = 0;
	// Display a string
	for(int i = 0; i < _content.length(); i++)
	{
		int ch = (int)_content.at(i).toLatin1();
		if(_font->getWidth(ch) > 0)
		{
			float tu, tv;
			tu = ((ch % 16) * _font->getSpace()) / 2048.0;
			tv = ((ch / 16) * _font->getSpace()) / 2048.0;


			int h, w;
			h = _font->getHeight();
			w = _font->getWidth(ch) * _w / widthContent;

			float tuW, tvH;
			tuW = tu + _font->getWidth(ch) / 2048.0;
			tvH = tv + h / 2048.0;

			//        (0,0) ------ (1,0)
			//          |            |
			//          |            |
			//        (0,1) ------ (1,1)

			glBegin(GL_QUADS); 	//Begining the cube's drawing
			{
				glTexCoord3f(tu, tv, 1);		glVertex3f(_x + x,		_y,	_z);
				glTexCoord3f(tuW, tv, 1);	    glVertex3f(_x + w + x,	_y,	_z);
				glTexCoord3f(tuW, tvH, 1);		glVertex3f(_x + w + x,	_y + h,  _z);
				glTexCoord3f(tu, tvH, 1);	    glVertex3f(_x + x,		_y + h,  _z);
			}
			glEnd();

		}
		// Shift the offset
		x += _font->getAdvance(ch);
	}


	glColor3f(_color.redF(), _color.greenF(), _color.blueF());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
}

