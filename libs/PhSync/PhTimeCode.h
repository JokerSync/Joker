/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHTIMECODE_H
#define PHTIMECODE_H

#include <QString>

#include "PhTime.h"

/**
 * @brief Different type of timecode.
 *
 * Enumerate the different type of timecode supported by the system.
 */
typedef enum {
	PhTimeCodeType2398,
	PhTimeCodeType24,
	PhTimeCodeType25,
	PhTimeCodeType2997,
	PhTimeCodeType30,
} PhTimeCodeType;

/** @brief Timecode representation utility.
 *
 * Provide tools for converting between frame, string representation and
 * BCD representation of a timecode value.
 */
class PhTimeCode
{
public:
	/**
	 * @brief Create a timecode string representation from a frame number and a type.
	 *
	 * @param frame A frame number.
	 * @param type A PhTimeCodeType value.
	 * @return An string* timecode representation.
	 */
	static QString stringFromFrame(PhFrame frame, PhTimeCodeType type);

	/**
	 * @brief Compute the frame number from a timecode string representation and a type.
	 *
	 * @param string A string.
	 * @param type A PhTimeCodeType value.
	 * @return The corresponding frame number.
	 */
	static PhFrame frameFromString(QString string, PhTimeCodeType type);

	/**
	 * @brief Compute the frame number from a timecode binary coded decimal (BCD) representation and a type.
	 *
	 * @param bcd A BCD value.
	 * @param type A PhTimeCodeType value.
	 * @return The corresponding frame number.
	 */
	static PhFrame frameFromBcd(unsigned int bcd, PhTimeCodeType type);

	/**
	 * @brief Compute the time value from a timecode binary coded decimal (BCD) representation and a type.
	 *
	 * @param bcd A BCD value.
	 * @param type A PhTimeCodeType value.
	 * @return A time value.
	 */
	static PhTime timeFromBcd(unsigned int bcd, PhTimeCodeType type);

	/**
	 * @brief Get the time per frame according to a type of TC
	 * @param type A timecode type
	 * @return A time value
	 */
	static PhTime timePerFrame(PhTimeCodeType type);

	/**
	 * @brief Compute the time value from a timecode string representation and a type.
	 * @param string A string
	 * @param type A timecode type
	 * @return A time value
	 */
	static PhTime timeFromString(QString string, PhTimeCodeType type);

	/**
	 * @brief Create a timecode string representation from a time value and a type.
	 * @param time A time value
	 * @param type A timecode type
	 * @return A string
	 */
	static QString stringFromTime(PhTime time, PhTimeCodeType type);

	/**
	 * @brief Create a timecode BCD representation from a frame number and a type.
	 *
	 * @param frame A frame number.
	 * @param type A timecode type value.
	 * @return A timecode BCD representation.
	 */
	static unsigned int bcdFromFrame(PhFrame frame, PhTimeCodeType type);

	/**
	 * @brief Create a timecode BCD representation from a time value and a type.
	 *
	 * @param time A time number.
	 * @param type A timecode type value.
	 * @return A timecode BCD representation.
	 */
	static unsigned int bcdFromTime(PhTime time, PhTimeCodeType type);

	/**
	 * @brief Compute the HH, MM, SS and FF timecode component from a frame number and a type.
	 *
	 * @param hhmmssff Array that will be filled with the corresponding HH, MM, SS, FF values.
	 * @param frame Frame number.
	 * @param type A timecode type.
	 */
	static void ComputeHhMmSsFf(unsigned int *hhmmssff, PhFrame frame, PhTimeCodeType type);

	/**
	 * @brief Compute the HH, MM, SS and FF timecode component from a time value and a type.
	 *
	 * @param hhmmssff Array that will be filled with the corresponding HH, MM, SS, FF values.
	 * @param time A time value.
	 * @param type A timecode type.
	 */
	static void ComputeHhMmSsFfFromTime(unsigned int *hhmmssff, PhTime time, PhTimeCodeType type);

	/**
	 * @brief Compute frame number from the HH, MM, SS and FF timecode component and a type.
	 *
	 * @param hh The hour value.
	 * @param mm The minute value.
	 * @param ss The second value.
	 * @param ff The frame value.
	 * @param type A timecode type.
	 * @return The corresponding frame number.
	 */
	static PhFrame frameFromHhMmSsFf(unsigned int hh, unsigned int mm, unsigned int ss, unsigned int ff, PhTimeCodeType type);

	/**
	 * @brief Compute frame number from the HH, MM, SS and FF timecode component and a type.
	 *
	 * @param hhmmssff Array containing the HH, MM, SS, FF values.
	 * @param type A timecode type.
	 * @return The corresponding frame number.
	 */
	static PhFrame frameFromHhMmSsFf(unsigned int * hhmmssff, PhTimeCodeType type);

	/**
	 * @brief Compute time from the HH, MM, SS and FF timecode component and a type.
	 *
	 * @param hh The hour value.
	 * @param mm The minute value.
	 * @param ss The second value.
	 * @param ff The frame value.
	 * @param type A timecode type.
	 * @return The corresponding time value.
	 */
	static PhFrame timeFromHhMmSsFf(unsigned int hh, unsigned int mm, unsigned int ss, unsigned int ff, PhTimeCodeType type);

	/**
	 * @brief Compute time from the HH, MM, SS and FF timecode component and a type.
	 *
	 * @param hhmmssff Array containing the HH, MM, SS, FF values.
	 * @param type A timecode type.
	 * @return The corresponding time number.
	 */
	static PhFrame timeFromHhMmSsFf(unsigned int * hhmmssff, PhTimeCodeType type);

	/**
	 * @brief Check if the timecode type is dropframe.
	 *
	 * @param type A timecode type.
	 * @return true if the timecode type is dropframe.
	 */
	static bool isDrop(PhTimeCodeType type);

	/**
	 * @brief Get the timecode type integer fps
	 *
	 * @param type A timecode type.
	 * @return Amount of frame per second.
	 */
	static PhFrame getFps(PhTimeCodeType type);

	/**
	 * @brief Get the timecode type average fps
	 *
	 * @param type A timecode type.
	 * @return The average frame per second.
	 */
	static float getAverageFps(PhTimeCodeType type);

	/**
	 * @brief Compute the timecode type from a average fps value
	 * @param averageFps A float value
	 * @return A timecode type
	 */
	static PhTimeCodeType computeTimeCodeType(float averageFps);
};

#endif // PHTIMECODE_H
