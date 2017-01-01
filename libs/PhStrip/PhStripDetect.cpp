/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripDetect.h"

PhStripDetect::PhStripDetect(PhDetectType type, PhTime relativeTime)
	: _type(type),
	  _relativeTime(relativeTime)
{

}

PhStripDetect::PhStripDetect()
	: _type(PhStripDetect::Unknown),
	  _relativeTime(0)
{

}

PhTime PhStripDetect::relativeTime() const
{
	return _relativeTime;
}

void PhStripDetect::setRelativeTime(const PhTime &time)
{
	_relativeTime = time;
}

void PhStripDetect::setType(const PhDetectType &type)
{
	_type = type;
}
