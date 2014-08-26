/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPDETECT_H
#define PHSTRIPDETECT_H

#include "PhSync/PhClock.h"

/**
 * @brief Block of detection during which a PhPeople is speaking.
 *
 * The block can be off (out of the picture) or not.
 */
class PhStripDetect : QObject
{
	Q_OBJECT

public:
	/**
	 * @brief The various type of detect
	 */
	enum PhDetectType {
		Unknown,
		On,
		SemiOff,
		Off,
		MouthOpen,
		MouthClosed,
		Aperture,
		Advance,
		Labial,
		SemiLabial,
		Bowl,
		Dental,
		ArrowUp,
		ArrowDown,
		AmbianceStart,
		AmbianceEnd,
	};

	/**
	 * @brief PhStripDetect constructor
	 * @param type The type of detect
	 * @param relativeTime The starting time of the detect, relative to the line start
	 */
	PhStripDetect(PhDetectType type, PhTime relativeTime);

	/**
	 * @brief If the people is out of the picture
	 * @return True if out of the picture, false otherwise
	 */
	PhDetectType type() {
		return _type;
	}

	void setType(const PhDetectType &type);

	PhTime relativeTime() const;
	void setRelativeTime(const PhTime &relativeTime);

private:
	PhDetectType _type;
	PhTime _relativeTime;
};

#endif // PHSTRIPDETECT_H
