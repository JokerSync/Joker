/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include "PhStripObject.h"

PhStripObject::PhStripObject()
{
	_timeIn = 0;
}

PhStripObject::PhStripObject(PhTime timeIn)
{
	_timeIn = timeIn;
}

PhTime PhStripObject::getTimeIn() {
	return _timeIn;
}

void PhStripObject::setTimeIn(PhTime timeIn)
{
	_timeIn = timeIn;
}
