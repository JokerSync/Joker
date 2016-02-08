#ifndef PHSTRIPSENTENCE_H
#define PHSTRIPSENTENCE_H

#include "PhStripPeopleObject.h"
#include "PhStripText.h"

class PhStripSentence : public PhStripText
{
public:
	PhStripSentence(PhStripText *text);

	void add(PhStripText *text);
private:
	QList<PhStripText*> _texts;
};

#endif // PHSTRIPSENTENCE_H
