/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhPeople.h"

PhPeople::PhPeople()
{
    _name = "Default";
    _color = "";
}

PhPeople::PhPeople(PhString name, PhColor color)
{
    _name = name;
    _color = color;
}

PhString PhPeople::getName()
{
    return _name;
}


PhColor PhPeople::getColor()
{
    return _color;
}
