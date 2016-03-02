/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOSETTINGS_H
#define PHVIDEOSETTINGS_H

#include "PhSync/PhTime.h"

/**
 * @brief The settings for the PhVideoEngine
 */
class PhVideoSettings
{
public:
	/**
	 * @brief PhVideoSettings desctructor
	 */
	virtual ~PhVideoSettings() {
	}

	/**
	 * @brief Display video in its native size
	 * @return True if native, false otherwise
	 */
	virtual bool useNativeVideoSize() = 0;

	/**
	 * @brief Number of frame read head by the engine
	 * @return An amount of frame
	 */
	virtual int videoReadhead() = 0;

	/**
	 * @brief Window (in frame number) within which frame are kept in the pool
	 * @return A frame number
	 */
	virtual PhFrame videoPoolSize() = 0;
};

#endif // PHVIDEOSETTINGS_H
