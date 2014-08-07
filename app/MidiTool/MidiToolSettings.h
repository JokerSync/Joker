#ifndef MIDITOOLSETTINGS_H
#define MIDITOOLSETTINGS_H

#include "PhTools/PhGenericSettings.h"

/**
 * @brief The MidiTool application settings
 */
class MidiToolSettings : protected PhGenericSettings
{
public:
	PH_SETTING_BOOL2(setGenerate, generate, true)
	PH_SETTING_BOOL2(setRead, read, true)

	PH_SETTING_INT2(setFirstFrame, firstFrame, 25 * 60 * 60)
	PH_SETTING_INT2(setLength, length, 25 * 60)

	PH_SETTING_STRING2(setInputPortName, inputPortName, "PhMidi In2")
	PH_SETTING_STRING2(setOutputPortName, outputPortName, "PhMidi In2")

	PH_SETTING_INT2(setLogMask, logMask, 1)
};

#endif // MIDITOOLSETTINGS_H
