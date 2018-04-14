#ifndef WINDOWSPECSETTINGS_H
#define WINDOWSPECSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhWindowSettings.h"

class WindowSpecSettings : public PhGenericSettings, public PhWindowSettings
{
	Q_OBJECT

public:
	WindowSpecSettings() : PhGenericSettings(true) {
	}

	// PhWindowSettings
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)
};

#endif // WINDOWSPECSETTINGS_H
