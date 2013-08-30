#ifndef PHSTRIPOFF_H
#define PHSTRIPOFF_H

#include "PhPeople.h"
#include "PhStrip/PhStripPeopleObject.h"

/**
 * Sentence or part of a sentence of a PhPeople with synchronization information.
 */
class PhStripOff : public PhStripPeopleObject {

public:


	PhStripOff( PhTime timeIn, PhPeople * people, PhTime timeOut, int track);

};

#endif // PHSTRIPOFF_H
