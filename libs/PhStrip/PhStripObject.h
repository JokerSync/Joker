#ifndef PHSTRIPOBJECT_H
#define PHSTRIPOBJECT_H

#include "PhTools/phtime.h"

/**
  * Generic object of a rythmo strip (loop, cut, text, ...).
  */
class PhStripObject {

private:
    /**
     * Starting time of the object.
     */
    PhTime _timeIn;
public:
    PhStripObject();
    PhStripObject(PhTime time);
};

#endif // PHSTRIPOBJECT_H
