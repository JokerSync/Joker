/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHLTCREADERSETTINGS_H
#define PHLTCREADERSETTINGS_H

#include "PhTools/PhGenericSettings.h"

class PhLtcReaderSettings
{
public:

	virtual bool autoFPSDetection() = 0;
	virtual void setAutoFPSDetection(bool enabled) = 0;

};

#endif // PHLTCREADERSETTINGS_H
