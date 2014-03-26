/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include "PhStripObject.h"

PhStripObject::PhStripObject(PhFrame frameIn) : _frameIn(frameIn)
{
}

PhFrame PhStripObject::frameIn() {
	return _frameIn;
}
