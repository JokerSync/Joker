/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripCut.h"

PhStripCut::PhStripCut(PhTime time, PhStripCut::PhCutType type) : PhStripObject(time)
{
	_type = type;
}
