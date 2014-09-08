/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHLTCREADERSETTINGS_H
#define PHLTCREADERSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhTimeCode.h"

/**
 * @brief The PhLtcReaderSettings class
 */
class PhLtcReaderSettings
{
public:

	/**
	 * @brief ltc AutoDetect TimeCodeType
	 * @return true if the auto detect mode is enabled, false otherwise
	 */
	virtual bool ltcAutoDetectTimeCodeType() = 0;

	/**
	 * @brief Hold the timecode type of the LTC reader
	 * @return the TimeCode Type
	 */
	virtual int ltcReaderTimeCodeType() = 0;
	/**
	 * @brief ltcInputDevice
	 * @return The name of the input device
	 */
	virtual QString ltcInputPort() = 0;

};

#endif // PHLTCREADERSETTINGS_H
