#ifndef SERIALTESTSETTINGS_H
#define SERIALTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
class SerialTestSettings : PhGenericSettings
{
public:
	PH_SETTING_STRING2(setPortAName, portAName, "A")
	PH_SETTING_STRING2(setPortBName, portBName, "B")

	PH_SETTING_BOOL2(setActivatePortA, activatePortA, true)
	PH_SETTING_BOOL2(setActivatePortB, activatePortB, true)
};

#endif // SERIALTESTSETTINGS_H
