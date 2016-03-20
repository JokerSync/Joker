/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QFont>
#include <QFontMetrics>
#include <QPixmap>
#include <QPainter>

#include "PhGraphicObject.h"
#include "PhFont.h"
#include "PhTools/PhDebug.h"

PhFont::PhFont() : _texture(-1), _glyphHeight(0), _weight(400), _ready(false)
{
	for(int ch = 0; ch < 256; ++ch) {
		_glyphAdvance[ch] = 0;
	}
}

bool PhFont::ready()
{
	return _ready;
}

void PhFont::setFamily(QString family)
{
	if(family != this->_family) {
		PHDEBUG << family;
		this->_family = family;
		_ready = false;
	}
}

QString PhFont::family()
{
	return _family;
}

int PhFont::computeMaxFontSize(QString family)
{
	int size = 25;
	int expectedFontHeight = 128;
	int low = 0, high = 1000;
	while (low < high) {
		size = (low + high) / 2;
		QFont font(family, size);
		QFontMetrics fm(font);

		PHDEBUG << fm.height() << fm.ascent() << fm.descent() << fm.leading();
		int computedFontHeight = fm.height();

		if (expectedFontHeight == computedFontHeight)
			break;
		else if (expectedFontHeight < computedFontHeight)
			high = size - 1;
		else
			low = size + 1;
	}

	return size;
}

QString PhFont::filter(QString inputText)
{
	QString outputText = "";

	foreach(QChar c, inputText) {
		switch (c.unicode()) {
		case 339:
			outputText += "œ";
			break;
		case 8216:
		case 8217:
			outputText += "'";
			break;
		case 8230:
			outputText += "...";
			break;
		default:
			outputText += c;
			break;
		}
	}

	return outputText;
}

bool PhFont::init()
{
	int size = computeMaxFontSize(_family);

	if(size < 0)
		return false;
	PHDEBUG << "Opening" << _family<< "at size" << size;
	QFont font(_family);
	font.setPixelSize(size);
	font.setWeight(_weight);
	QFontMetrics fm(font);
	PHDEBUG << "font height:" << fm.height();

	QPixmap pixmap(2048, 2048);

	// Font background color is transparent
	pixmap.fill(Qt::transparent);

	QPainter painter(&pixmap);
	painter.setPen(Qt::white);
	painter.setFont(font);

	// Space between glyph
	int space = 128;
	_glyphHeight = fm.height();

	// We get rid of the 32 first useless char
	for(int i = 32; i < 256; i++) {
		QChar c = QChar::fromLatin1(i);
		QString charString = c;
		int width = fm.width(charString);
		_glyphAdvance[i] = width;
		int x = (i % 16) * space + space / 2 - width / 2;
		int y = (i / 16) * space + fm.ascent();
		// First render the glyph to a surface
		painter.drawText(x, y, charString);
	}

	pixmap.save("/Users/martin/Desktop/pixmap.png");

	glEnable( GL_TEXTURE_2D );
	// Have OpenGL generate a texture object handle for us
	glGenTextures( 1, &_texture );

	// Bind the texture object
	glBindTexture( GL_TEXTURE_2D, _texture );

	// Edit the texture object's image data
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, pixmap.width(), pixmap.height(), 0,
	              GL_RGBA, GL_UNSIGNED_BYTE, pixmap.toImage().bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	_ready = true;
	return _ready;
}

int PhFont::getAdvance(unsigned char ch)
{
	return _glyphAdvance[ch];
}

void PhFont::select()
{
	if(!_ready)
		this->init();
	glBindTexture(GL_TEXTURE_2D, (GLuint)_texture);
}

int PhFont::weight() const
{
	return _weight;
}

int PhFont::getNominalWidth(QString string)
{
	if(!_ready)
		this->init();
	int width = 0;
	foreach(QChar c, filter(string)) {
		width += getAdvance(c.toLatin1());
	}
	return width;
}

void PhFont::setWeight(int weight)
{
	if(_weight != weight) {
		_weight = weight;
		_ready = false;
	}
}

