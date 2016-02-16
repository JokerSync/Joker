/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOSETTINGS_H
#define PHVIDEOSETTINGS_H

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
	 * @brief Screen delay compensation
	 * @return A value in millisecond
	 */
	virtual int screenDelay() = 0;

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
	 * @brief Time window within which frame are kept in the pool
	 * @return A duration
	 */
	virtual PhTime videoPoolWindow() = 0;
};

#endif // PHVIDEOSETTINGS_H
