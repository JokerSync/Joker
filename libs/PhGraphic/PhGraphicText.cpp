/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhGraphicText.h"

PhGraphicText::PhGraphicText(PhFont* font, QString content, int x, int y, int w, int h)
	: PhGraphicRect(x, y, w, h), _font(font), _content("")
{
	setContent(content);
}

void PhGraphicText::setContent(QString content)
{
	_content = PhFont::filter(content);
}

void PhGraphicText::setFont(PhFont * font)
{
	_font = font;
}

QString PhGraphicText::content()
{
	return _content;
}

PhFont * PhGraphicText::getFont()
{
	return _font;
}

void PhGraphicText::draw()
{
	PhGraphicRect::draw();

	_font->select();

	int totalAdvance = 0;
	//Compute the natural width of the content to scale it later
	for(int i = 0; i < _content.length(); i++) {
		totalAdvance += _font->getAdvance(_content.at(i).toLatin1());
		if(_content.at(i).unicode() == 339)
			totalAdvance += _font->getAdvance(153);
	}

	if((totalAdvance == 0) || (_font->height() == 0)) {
		// empty string or bad font initialization: displaying a rect
		glBegin(GL_QUADS);
		{
			glVertex3i(this->x(),      this->y(), this->z());
			glVertex3i(this->x() + this->width(),  this->y(), this->z());
			glVertex3i(this->x() + this->width(),  this->y() + this->height(),  this->z());
			glVertex3i(this->x(),      this->y() +this->height(),  this->z());
		}
		glEnd();

		return;
	}

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	// Set the letter initial horizontal offset
	int currentAdvance = 0;
	float space = 0.0625f; // all glyph are in a 1/16 x 1/16 box
	// Display a string
	for(int i = 0; i < _content.length(); i++) {
		QChar qChar = _content.at(i);
		unsigned char ch = (unsigned char)qChar.toLatin1();
		int glyphAdvance = _font->getAdvance(ch);
		if (ch == 0)
			PHERR << "Unhandled character:" << qChar << "/" << qChar.unicode();
		else if(glyphAdvance) {
			// computing texture coordinates
			float tu1 = (ch % 16) * space;
			float tv1 = (ch / 16) * space;
			float tu2 = tu1 + space;
			float tv2 = tv1 + space;

			// computing quads coordinate;
			int h = this->height();
			int w = this->width() * 128 / totalAdvance;

			//        (tu1, tv1) --- (tu2, tv1)
			//            |              |
			//            |              |
			//        (tu1, tv2) --- (tu2, tv2)


			int offset = (128 - glyphAdvance) / 2 * this->width() / totalAdvance;
			int glyphX = this->x() + currentAdvance * this->width() / totalAdvance - offset;
			glBegin(GL_QUADS);  //Begining the cube's drawing
			{
				glTexCoord3f(tu1, tv1, 1);  glVertex3i(glyphX,      this->y(), this->z());
				glTexCoord3f(tu2, tv1, 1);  glVertex3i(glyphX + w,  this->y(), this->z());
				glTexCoord3f(tu2, tv2, 1);  glVertex3i(glyphX + w,  this->y() + h,  this->z());
				glTexCoord3f(tu1, tv2, 1);  glVertex3i(glyphX,      this->y() + h,  this->z());
			}
			glEnd();

		}
		// Inc the advance
		currentAdvance += glyphAdvance;
	}

	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
}

