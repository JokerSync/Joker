#ifndef PHTIMECODE_H
#define PHTIMECODE_H

#include "PhTime.h"

/** Different type of timecode

 Enumerate the different type of timecode supported by the system.
 */
typedef enum {
    PhTimeCodeType2398,
    PhTimeCodeType24,
    PhTimeCodeType25,
    PhTimeCodeType2997
} PhTimeCodeType;


/** Timecode representation utility

 Provide tools for converting between frame, string representation and
 BCD representation of a timecode value.
 */
class PhTimeCode
{
public:
    /**
     Create a timecode string representation from a frame number and a type
     @param frame A frame number.
     @param type A PhTimeCodeType value.
     @return An string* timecode representation.
     */
	static QString stringFromFrame(PhFrame frame, PhTimeCodeType type);

    /**
     Compute the frame number from a timecode string representation and a type
     @param string A string.
     @param type A PhTimeCodeType value.
     @return The corresponding frame number.
     */
	static PhFrame frameFromString(QString string, PhTimeCodeType type);

    /**
     Compute the frame number from a timecode binary coded decimal (BCD) representation and a type
     @param bcd A BCD value.
     @param type A PhTimeCodeType value.
     @return The corresponding frame number.
     */
    static PhFrame frameFromBcd(unsigned int bcd, PhTimeCodeType type);

    /**
     Create a timecode BCD representation from a frame number and a type
     @param frame A frame number.
     @param type A PhTimeCodeType value.
     @return A timecode BCD representation.
     */
    static unsigned int bcdFromFrame(PhFrame frame, PhTimeCodeType type);

    /**
     Compute the HH, MM, SS and FF timecode component from a frame number and a type.
     @param hh Hours output value
     @param mm Minutes output value
     @param ss Secondes output value
     @param ff Frame output value
     @param frame Frame number
     @param type Timecode typ
     */
    static void ComputeHhMmSsFf(unsigned int *hhmmssff, PhFrame frame, PhTimeCodeType type);

    /**
     Compute frame number from the HH, MM, SS and FF timecode component and a type.
     @param hh Hours output value
     @param mm Minutes output value
     @param ss Secondes output value
     @param ff Frame output value
     @param type Timecode typ
     @return A frame number
     */
    static PhFrame frameFromHhMmSsFf(unsigned int * hhmmssff, PhTimeCodeType type);

    /**
     * @brief check if the timecode type is dropframe
     * @param type of the timecode
     * @return if the timecode type is dropframe
     */
    static bool isDrop(PhTimeCodeType type);

    /**
     * @brief check if the timecode type frame per second
     * @param type of the timecode
     * @return amount of frame per second
     */
    static PhFrame getFps(PhTimeCodeType type);
};

#endif // PHTIMECODE_H
