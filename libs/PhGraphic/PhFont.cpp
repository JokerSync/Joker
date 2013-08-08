/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QStringList>
#include "PhFont.h"

PhFont::PhFont(PhString filename, int size)
{
    _filename = filename;
    _size = size;
}

TTF_Font * PhFont::getFont()
{
    return _font;
}

PhString PhFont::getFontName(){
    QStringList list ;
    list = _filename.split("/");
    PhString name = list.last().split(".").first();
	return name;
}

bool PhFont::init()
{
	_font = TTF_OpenFont(_filename.toStdString().c_str(), _size);
	return(_font != NULL);
}
