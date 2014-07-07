/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhPeople.h"

PhPeople::PhPeople(QString name, QString color)
{
	_name = name;
	_color = color;
}

QString PhPeople::name()
{
	return _name;
}


QString PhPeople::color()
{
	return _color;
}

void PhPeople::setColor(QString color)
{
	_color = color;
}
