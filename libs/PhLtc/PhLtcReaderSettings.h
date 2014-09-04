/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHLTCREADERSETTINGS_H
#define PHLTCREADERSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhTimeCode.h"

class PhLtcReaderSettings
{
public:

	virtual bool ltcAutoDetectTimeCodeType() = 0;

	virtual PhTimeCodeType ltcReaderTimeCodeType() = 0;
	virtual QString ltcInputDevice() = 0;

};

#endif // PHLTCREADERSETTINGS_H
