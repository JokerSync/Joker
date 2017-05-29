#ifndef STRIPTESTSETTINGS_H
#define STRIPTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"

class StripTestSettings : protected PhGenericSettings
{
	Q_OBJECT

public:
	PH_SETTING_INT2(setLogMask, logMask, 1);
};

#endif // STRIPTESTSETTINGS_H
