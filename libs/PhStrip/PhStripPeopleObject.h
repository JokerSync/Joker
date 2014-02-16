/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPPEOPLEOBJECT_H
#define PHSTRIPPEOPLEOBJECT_H

#include "PhPeople.h"
#include "PhStrip/PhStripObject.h"

/**
 * @brief The PhStripPeopleObject class
 */
class PhStripPeopleObject : public PhStripObject {

public:

	/**
	 * @brief PhStripPeopleObject constructor
	 * @param timeIn the beggining of the PhStripPeopleObject
	 * @param people the corresponding PhPeople
	 * @param timeOut the end of the PhStripPeopleObject
	 * @param track the track of the PhStripPeopleObject
	 */
	PhStripPeopleObject(PhTime timeIn, PhPeople * people = NULL, PhTime timeOut = 0, int track = 0);
	/**
	 * @brief Get the PhPeople
	 * @return _people the corresponding PhPeople
	 */
	PhPeople * getPeople(){
		return _people;
	}
	/**
	 * @brief Get the track
	 * @return _track the corresponding track
	 */
	int getTrack(){
		return _track;
	}
	/**
	 * @brief getTimeOut
	 * @return _timeOut
	 */
	int getTimeOut(){
		return _timeOut;
	}
	/**
	 * @brief Set the PhPeople
	 * @param people
	 */
	void setPeople(PhPeople * people){
		_people = people;
	}
	/**
	 * @brief Set the track
	 * @param track
	 */
	void setTrack(int track){
		_track = track;
	}
	/**
	 * @brief Set the time out
	 * @param timeOut
	 */
	void setTimeOut(int timeOut){
		_timeOut = timeOut;
	}

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
