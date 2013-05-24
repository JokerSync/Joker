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
