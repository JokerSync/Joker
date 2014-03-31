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
 * @brief A strip object affected to a PhPeople
 *
 * Its properties are people, track and timeOut.
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
	PhPeople * people() {
		return _people;
	}
	/**
	 * @brief The object track
	 * @return _track An integer
	 */
	int track() {
		return _track;
	}
	/**
	 * @brief The time out
	 * @return A time value
	 */
	PhTime timeOut() {
		return _timeOut;
	}
	/**
	 * @brief Affect a people
	 * @param people
	 */
	void setPeople(PhPeople * people) {
		_people = people;
	}
	/**
	 * @brief Set the track
	 * @param track An integer
	 */
	void setTrack(int track) {
		_track = track;
	}
	/**
	 * @brief Set the time out
	 * @param timeOut A time
	 */
	void setTimeOut(PhTime timeOut) {
		_timeOut = timeOut;
	}

private:
/**
 * Indicate the end of the PhText. The text content is stretch between timeIn and timeOut
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
