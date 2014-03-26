/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPDETECT_H
#define PHSTRIPDETECT_H

#include "PhStripPeopleObject.h"

/**
 * @brief Block of detection during which a PhPeople is speaking.
 *
 * The block can be off (out of the picture) or not.
 */
class PhStripDetect : public PhStripPeopleObject
{
public:
	/**
	 * @brief PhStripOff constructor
	 * @param off If the people is out of the picture
	 * @param frameIn the beggining of the PhStripOff
	 * @param people the corresponding PhPeople
	 * @param frameOut the end of the PhStripOff
	 * @param track the track of the PhStripOff
	 */
	PhStripDetect( bool off, PhFrame frameIn, PhPeople * people, PhFrame frameOut, int track);

	/**
	 * @brief If the people is out of the picture
	 * @return True if out of the picture, false otherwise
	 */
	bool off() {
		return _off;
	}

private:
	bool _off;
};

#endif // PHSTRIPDETECT_H
