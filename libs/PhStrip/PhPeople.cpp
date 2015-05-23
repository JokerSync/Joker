/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhPeople.h"

PhPeople::PhPeople(QString name, QString color, QString picture) : _name(name), _color(color), _picture(picture)
{
}

QString PhPeople::name() const
{
	return _name;
}


QString PhPeople::color() const
{
	return _color;
}

void PhPeople::setColor(QString color)
{
	_color = color;
}

QString PhPeople::picture() const
{
	return _picture;
}

void PhPeople::setPicture(QString picture)
{
	_picture = picture;
}
