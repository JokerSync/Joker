/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripDetect.h"

PhStripDetect::PhStripDetect(bool off, PhFrame frameIn, PhPeople *people, PhFrame frameOut, int track)
	: PhStripPeopleObject(frameIn, people, frameOut, track),
	_off(off)
{

}
