#ifndef PHSTRIPTEXT_H
#define PHSTRIPTEXT_H

#include "PhPeople.h"
#include "PhStrip/PhStripPeopleObject.h"

/**
 * Sentence or part of a sentence of a PhPeople with synchronization information.
 */
class PhStripText : public PhStripPeopleObject {

public:

    /**
     * @brief PhStripText
     * @param people
     * @param content
     * @param timeIn
     * @param timeOut
     * @param track
     * @param simpleText
     * Constructor
     */
	PhStripText( PhTime timeIn, PhPeople * people, PhTime timeOut, int track,  QString content);
    /**
     * @brief getContent
     * @return _content
     */
	QString getContent();

	void setContent(QString content);

private:

/**
 * Text string of Sentence or part of a sentence.
 */
	QString _content;


};

#endif // PHSTRIPTEXT_H
