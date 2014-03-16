#ifndef AUTOTESTSETTINGS_H
#define AUTOTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class AutoTestSettings : public PhGenericSettings, public PhWindowSettings
{
public:
	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)
};

#endif // AUTOTESTSETTINGS_H
