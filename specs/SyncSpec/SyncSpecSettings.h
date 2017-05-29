/**
 * Copyright (C) 2012-2015 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef SYNCSPECSETTINGS_H
#define SYNCSPECSETTINGS_H



#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhSyncSettings.h"

class SyncSpecSettings : protected PhGenericSettings,
		public PhSyncSettings
{
	Q_OBJECT

public:
	SyncSpecSettings() : PhGenericSettings(true) {
	}

	// Sync settings
	PH_SETTING_TIME(setSyncLoopTimeIn, syncLoopTimeIn)
	PH_SETTING_TIME(setSyncLoopTimeOut, syncLoopTimeOut)
	PH_SETTING_BOOL(setSyncLooping, syncLooping)
};

#endif // SYNCSPECSETTINGS_H
