#include "PhStrip/PhStripPeopleObject.h"


PhStripPeopleObject::PhStripPeopleObject(PhTime timeIn, PhPeople *people, PhTime timeOut, int track): PhStripObject(timeIn),
	_people(people), _timeOut(timeOut), _track(track)
{
}


