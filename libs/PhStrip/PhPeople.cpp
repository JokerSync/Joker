/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhPeople.h"

PhPeople::PhPeople(QString name, QString color)
	: QObject()
{
	_name = name;
	_color = color;
}

QString PhPeople::name()
{
	return _name;
}

void PhPeople::setName(QString name)
{
	if(name != _name)
		emit nameChanged();

	_name = name;
}

QString PhPeople::color()
{
	return _color;
}

void PhPeople::setColor(QString color)
{
	if(color != _color)
		emit colorChanged();

	_color = color;
}
