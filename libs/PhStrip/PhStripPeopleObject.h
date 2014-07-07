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
	 * @param y the track of the PhStripPeopleObject
	 * @param height the track height
	 */
	PhStripPeopleObject(PhTime timeIn, PhPeople * people = NULL, PhTime timeOut = 0, float y = 0, float height = 0.25f);
	/**
	 * @brief Get the PhPeople
	 * @return _people the corresponding PhPeople
	 */
	PhPeople * people() {
		return _people;
	}
	/**
	 * @brief The object track
	 * @return _y An integer
	 */
	float y() {
		return _y;
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
	 * @brief Set the y position of the text on the strip
	 * @param y An float between 0 and 1
	 */
	void setY(float y) {
		_y = y;
	}
	/**
	 * @brief Set the time out
	 * @param timeOut A time
	 */
	void setTimeOut(PhTime timeOut) {
		_timeOut = timeOut;
	}
	/**
	 * @brief Height of the text
	 * @return the fraction of the track corresponding to the height
	 */
	float height() const;
	/**
	 * @brief Set the height of the text
	 * @param height the fraction desired of the track height
	 */
	void setHeight(float height);

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
	float _y;

	float _height;
};

#endif // PHSTRIPPEOPLEOBJECT_H
