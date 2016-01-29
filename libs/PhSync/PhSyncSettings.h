/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSYNCSETTINGS_H
#define PHSYNCSETTINGS_H

#include "PhTime.h"

/** Implement the time setter and getter for a PhSyncSettings */
#define PH_SETTING_TIME(setter, getter) \
public slots: \
	void setter(PhTime getter) { setLongLongValue(#getter, getter); } \
public: \
	PhTime getter() {return (PhTime)longLongValue(#getter); }


/**
 * @brief The settings for PhSync
 */
class PhSyncSettings
{
public:
	/**
	 * @brief PhSyncSettings destructor
	 */
	virtual ~PhSyncSettings() {
	}

	/**
	 * @brief Time to start playback when looped
	 * @return A time value
	 */
	virtual PhTime syncLoopTimeIn() = 0;

	/**
	 * @brief Time to loop back playback when looped
	 * @return A time value
	 */
	virtual PhTime syncLoopTimeOut() = 0;

	/**
	 * @brief Is the playback looping?
	 * @return True if looping, false otherwise
	 */
	virtual bool syncLooping() = 0;
};

#endif // PHSYNCSETTINGS
