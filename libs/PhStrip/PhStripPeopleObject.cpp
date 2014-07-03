/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStrip/PhStripPeopleObject.h"


PhStripPeopleObject::PhStripPeopleObject(PhTime timeIn, PhPeople *people, PhTime timeOut, float y, float height) : PhStripObject(timeIn),
	_timeOut(timeOut), _people(people), _y(y), _trackHeight(height)
{
}
float PhStripPeopleObject::trackHeight() const
{
	return _trackHeight;
}

void PhStripPeopleObject::setTrackHeight(float trackHeight)
{
	_trackHeight = trackHeight;
}



