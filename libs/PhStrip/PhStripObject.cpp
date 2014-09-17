/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include "PhStripObject.h"

PhStripObject::PhStripObject(PhTime timeIn)
  : QObject(),
	_timeIn(timeIn)
{
}

PhTime PhStripObject::timeIn() {
	return _timeIn;
}

void PhStripObject::setTimeIn(PhTime timeIn)
{
	if (timeIn != _timeIn) {
		_timeIn = timeIn;
		emit timeInChanged();
	}
}

bool PhStripObject::dtcomp(PhStripObject *a, PhStripObject *b)
{
	return a->_timeIn < b->_timeIn;
}
