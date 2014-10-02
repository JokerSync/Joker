/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef VIDEOSPECSETTINGS_H
#define VIDEOSPECSETTINGS_H




#include "PhVideo/PhVideoSettings.h"

class VideoSpecSettings : public PhVideoSettings
{
public:
	//PhVideoSettings
	int screenDelay() {
		return 0;
	}

};

#endif // VIDEOSPECSETTINGS_H
