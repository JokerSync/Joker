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
 * Its properties are people, track and frameOut.
 */
class PhStripPeopleObject : public PhStripObject {

public:

	/**
	 * @brief PhStripPeopleObject constructor
	 * @param frameIn the beggining of the PhStripPeopleObject
	 * @param people the corresponding PhPeople
	 * @param frameOut the end of the PhStripPeopleObject
	 * @param track the track of the PhStripPeopleObject
	 */
	PhStripPeopleObject(PhFrame frameIn, PhPeople * people = NULL, PhFrame frameOut = 0, int track = 0);
	/**
	 * @brief Get the PhPeople
	 * @return _people the corresponding PhPeople
	 */
	PhPeople * people() {
		return _people;
	}
	/**
	 * @brief Get the track
	 * @return _track the corresponding track
	 */
	int track() {
		return _track;
	}
	/**
	 * @brief The frame out
	 * @return A frame value
	 */
	PhFrame frameOut() {
		return _frameOut;
	}
	/**
	 * @brief Set the PhPeople
	 * @param people
	 */
	void setPeople(PhPeople * people) {
		_people = people;
	}
	/**
	 * @brief Set the track
	 * @param track
	 */
	void setTrack(int track) {
		_track = track;
	}
	/**
	 * @brief Set the frame out
	 * @param A frame
	 */
	void setFrameOut(int frameOut) {
		_frameOut = frameOut;
	}

private:
/**
 * Indicate the end of the PhText. The text content is stretch between frameIn and frameOut
 */
	PhFrame _frameOut;

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
