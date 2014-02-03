#include "PhStrip/PhStripPeopleObject.h"


PhStripPeopleObject::PhStripPeopleObject(PhTime timeIn, PhPeople *people, PhTime timeOut, int track): PhStripObject(timeIn),
	_timeOut(timeOut), _people(people), _track(track)
{
}


