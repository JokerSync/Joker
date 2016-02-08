#ifndef PHSTRIPSENTENCE_H
#define PHSTRIPSENTENCE_H

#include "PhStripPeopleObject.h"
#include "PhStripText.h"

class PhStripSentence : public PhStripPeopleObject
{
public:
	PhStripSentence(PhStripText *text);
};

#endif // PHSTRIPSENTENCE_H
