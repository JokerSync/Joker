/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef VIDEOTESTSETTINGS_H
#define VIDEOTESTSETTINGS_H




#include "PhVideo/PhVideoSettings.h"

class VideoTestSettings : public PhVideoSettings
{
public:
	//PhVideoSettings
	int screenDelay() {
		return 0;
	}

};

#endif // VIDEOTESTSETTINGS_H
