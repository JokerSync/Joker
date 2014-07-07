/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripDetect.h"

PhStripDetect::PhStripDetect(PhDetectType type, PhTime timeIn, PhPeople *people, PhTime timeOut, float y)
	: PhStripPeopleObject(timeIn, people, timeOut, y),
	_type(type)
{

}
