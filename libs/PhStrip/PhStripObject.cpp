#include "PhStripObject.h"

PhStripObject::PhStripObject()
{
    _timeIn = 0;
}

PhStripObject::PhStripObject(PhTime timeIn)
{
    _timeIn = timeIn;
}

PhTime PhStripObject::getTimeIn(){
    return _timeIn;
}
