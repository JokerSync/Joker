#ifndef SERIALTESTSETTINGS_H
#define SERIALTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
class SerialTestSettings : PhGenericSettings
{
public:
	PH_SETTING_INT2(setDeviceNumberA, deviceNumberA, 0);
	PH_SETTING_INT2(setDeviceNumberB, deviceNumberB, 1);

	PH_SETTING_BOOL2(setActivatePortA, activatePortA, true)
	PH_SETTING_BOOL2(setActivatePortB, activatePortB, true)
};

#endif // SERIALTESTSETTINGS_H
