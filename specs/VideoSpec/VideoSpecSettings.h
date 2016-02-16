/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef VIDEOSPECSETTINGS_H
#define VIDEOSPECSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhVideo/PhVideoSettings.h"

class VideoSpecSettings : protected PhGenericSettings,
		public PhVideoSettings
{
public:
	VideoSpecSettings() : PhGenericSettings(true) {
	}

	//PhVideoSettings
	int screenDelay() {
		return 0;
	}

	PH_SETTING_BOOL(setUseNativeVideoSize, useNativeVideoSize)

	PH_SETTING_INT2(setVideoReadhead, videoReadhead, 5)
};

#endif // VIDEOSPECSETTINGS_H
