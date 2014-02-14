/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStrip/PhStripPeopleObject.h"


PhStripPeopleObject::PhStripPeopleObject(PhTime timeIn, PhPeople *people, PhTime timeOut, int track) : PhStripObject(timeIn),
	_timeOut(timeOut), _people(people), _track(track)
{
}


