#ifndef PHSTRIPPEOPLEOBJECT_H
#define PHSTRIPPEOPLEOBJECT_H

#include "PhPeople.h"
#include "PhStrip/PhStripObject.h"


class PhStripPeopleObject : public PhStripObject {

public:


	PhStripPeopleObject(PhTime timeIn, PhPeople * people = NULL, PhTime timeOut = 0, int track = 0);

	/**
	 * @brief getPeople
	 * @return _people
	 */
	PhPeople * getPeople(){return _people;};
	/**
	 * @brief getTrack
	 * @return _track
	 */
	int getTrack(){return _track;};

	/**
	 * @brief getTimeOut
	 * @return _timeOut
	 */
	int getTimeOut(){return _timeOut;};

	void setPeople(PhPeople * people){_people = people;};

	void setTrack(int track){_track = track;};

	void setTimeOut(int timeOut){_timeOut = timeOut;};

private:
/**
 * Indicate the end of the PhText. The text content is stretch between timeIn : PhTime and timeOut : PhTime
 */
	PhTime _timeOut;

	/**
	 * Reference to the PhPeople who is affected to the PhStripPeopleObject
	 */
	PhPeople *_people;

	/**
	 * @brief _track
	 * Track of the text, 0 is on top of the strip
	 */
	int _track;
};

#endif // PHSTRIPPEOPLEOBJECT_H
