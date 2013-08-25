/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhPeople.h"

PhPeople::PhPeople()
{
    _name = "???";
	_color = "#000000";
}

PhPeople::PhPeople(QString name, QString color)
{
    _name = name;
    _color = color;
}

QString PhPeople::getName()
{
    return _name;
}


QString PhPeople::getColor()
{
    return _color;
}
