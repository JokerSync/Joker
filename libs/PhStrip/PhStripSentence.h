#ifndef PHSTRIPSENTENCE_H
#define PHSTRIPSENTENCE_H

#include "PhStripPeopleObject.h"
#include "PhStripText.h"

/**
 * @brief Container of several concatenated PhStripText with the same PhPeople and height
 */
class PhStripSentence : public PhStripText
{
public:
	/**
	 * @brief PhStripSentence constructor
	 * @param text The first PhStripText
	 */
	PhStripSentence(PhStripText *text);

	/**
	 * @brief Add a new PhStripText to the container
	 * @param text A text
	 */
	void add(PhStripText *text);

	/**
	 * @brief List of texts
	 * @return A list
	 */
	QList<PhStripText*> texts();
private:
	QList<PhStripText*> _texts;
};

#endif // PHSTRIPSENTENCE_H
