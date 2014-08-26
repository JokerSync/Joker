/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStrip/PhStripPeopleObject.h"


PhStripPeopleObject::PhStripPeopleObject(PhTime timeIn, PhPeople *people, PhTime timeOut, float y, float height) : PhStripObject(timeIn),
	_timeOut(timeOut), _people(people), _y(y), _height(height)
{
}

QString PhStripPeopleObject::tcOut(PhTimeCodeType tcType)
{
	return PhTimeCode::stringFromTime(this->timeOut(), tcType);
}

float PhStripPeopleObject::height() const
{
	return _height;
}

void PhStripPeopleObject::setHeight(float height)
{
	if (height != _height) {
		_height = height;
		emit heightChanged();
	}
}

QString PhStripPeopleObject::peopleObjectDescription(PhTimeCodeType tcType)
{
	return QString("%1 %2: %4 %3")
	       .arg(this->people() ? this->people()->name() : "???")
	       .arg(this->tcIn(tcType))
	       .arg(this->tcOut(tcType));
}



