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
	Q_OBJECT

	Q_PROPERTY(int position READ position WRITE setPosition NOTIFY positionChanged)

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
	 * @param timeIn The starting time of the detect
	 * @param people the corresponding PhPeople
	 * @param timeOut The ending time of the detect
	 * @param y The track of the detect
	 */
	PhStripDetect(PhDetectType type, PhTime timeIn, PhPeople * people, PhTime timeOut, float y);

	/**
	 * @brief If the people is out of the picture
	 * @return True if out of the picture, false otherwise
	 */
	PhDetectType type() {
		return _type;
	}

	int position() {
		return _position;
	}

	void setPosition(int position) {
		if (position != _position) {
			_position = position;
			emit positionChanged();
		}
	}

signals:
	void positionChanged();

private:
	PhDetectType _type;
	int _position;
};

#endif // PHSTRIPDETECT_H
