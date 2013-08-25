/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhPeople.h"

PhPeople::PhPeople()
{
    _name = "Default";
	_color = new PhColor();
}

PhPeople::PhPeople(QString name, PhColor *color)
{
    _name = name;
    _color = color;
}

QString PhPeople::getName()
{
    return _name;
}


PhColor *PhPeople::getColor()
{
    return _color;
}
