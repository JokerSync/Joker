#ifndef PHSTRIPTEXT_H
#define PHSTRIPTEXT_H

#include "PhPeople.h"
#include "PhStrip/PhStripObject.h"

/**
 * Sentence or part of a sentence of a PhPeople with synchronization information.
 */
class PhStripText : public PhStripObject {

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
	PhStripText(PhPeople * people, QString content, PhTime timeIn, PhTime timeOut, int track);
    /**
     * @brief getContent
     * @return _content
     */
	QString getContent();
    /**
     * @brief getPeople
     * @return _people
     */
    PhPeople * getPeople();
    /**
     * @brief getTrack
     * @return _track
     */
    int getTrack();
    /**
     * @brief getTimeIn
     * @return _timeIn
     */
    int getTimeIn();
    /**
     * @brief getTimeOut
     * @return _timeOut
     */
    int getTimeOut();
    /**
     * @brief isSimple
     * @return _simpleText
     */
    bool isSimple();

private:
/**
 * Indicate the end of the PhText. The text content is stretch between timeIn : PhTime and timeOut : PhTime
 */
    PhTime _timeOut;
/**
 * Text string of Sentence or part of a sentence.
 */
	QString _content;

    /**
     * Reference to the PhPeople who is affected to the PhStripText
     */
    PhPeople *_people;

    /**
     * @brief _track
     * Track of the text, 0 is on top of the strip
     */
    int _track;
};

#endif // PHSTRIPTEXT_H
