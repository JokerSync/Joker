#include "PhStripCut.h"

PhStripCut::PhStripCut()
{
}

PhStripCut::PhStripCut(PhStripCut::PhCutType type, PhTime time) : PhStripObject(time)
{
    _type = type;
}
