/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStrip/PhStripPeopleObject.h"


PhStripPeopleObject::PhStripPeopleObject(PhFrame frameIn, PhPeople *people, PhFrame frameOut, int track) : PhStripObject(frameIn),
	_frameOut(frameOut), _people(people), _track(track)
{
}


