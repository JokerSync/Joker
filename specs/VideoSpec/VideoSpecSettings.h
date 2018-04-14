/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef VIDEOSPECSETTINGS_H
#define VIDEOSPECSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhSyncSettings.h"
#include "PhVideo/PhVideoSettings.h"

class VideoSpecSettings : protected PhGenericSettings,
		public PhVideoSettings
{
	Q_OBJECT

public:
	VideoSpecSettings() : PhGenericSettings(true) {
	}

	//PhVideoSettings
	int screenDelay() {
		return 0;
	}

	PH_SETTING_BOOL(setUseNativeVideoSize, useNativeVideoSize)

	PH_SETTING_INT2(setVideoReadhead, videoReadhead, 5)

	PH_SETTING_FRAME2(setVideoPoolSize, videoPoolSize, 50)
};

#endif // VIDEOSPECSETTINGS_H
