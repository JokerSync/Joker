/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripDetect.h"

PhStripDetect::PhStripDetect(PhDetectType type, PhTime timeIn, PhPeople *people, PhTime timeOut, float y, float height)
	: PhStripPeopleObject(timeIn, people, timeOut, y, height),
	_type(type)
{

}
