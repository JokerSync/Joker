/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhStripCut.h"

PhStripCut::PhStripCut(PhStripCut::PhCutType type, PhTime time) : PhStripObject(time)
{
    _type = type;
}
