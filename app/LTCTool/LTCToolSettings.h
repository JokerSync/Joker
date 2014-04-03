#ifndef LTCTOOLSETTINGS_H
#define LTCTOOLSETTINGS_H

#include "PhTools/PhGenericSettings.h"

class LTCToolSettings : protected PhGenericSettings
{
public:
	PH_SETTING_BOOL2(setGenerate, generate, true)
	PH_SETTING_BOOL2(setRead, read, true)

	PH_SETTING_INT2(setFirstFrame, firstFrame, 25 * 60 * 60)
	PH_SETTING_INT2(setLength, length, 25 * 60)

	PH_SETTING_STRING(setAudioOutput, audioOutput)
	PH_SETTING_STRING(setAudioInput, audioInput)

	PH_SETTING_INT2(setLogMask, logMask, 1)
};

#endif // LTCTOOLSETTINGS_H
