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
