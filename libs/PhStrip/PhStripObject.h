#ifndef PHSTRIPOBJECT_H
#define PHSTRIPOBJECT_H

#include "PhTools/PhTime.h"

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
    /**
     * @brief PhStripObject
     * default constructor
     */
    PhStripObject();
    /**
     * @brief PhStripObject
     * @param time
     * contructor
     */
    PhStripObject(PhTime time);
    /**
     * @brief getTimeIn
     * @return _timeIn
     */
    PhTime getTimeIn();
};

#endif // PHSTRIPOBJECT_H
