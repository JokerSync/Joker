#ifndef MIDITOOLSETTINGS_H
#define MIDITOOLSETTINGS_H

#include "PhTools/PhGenericSettings.h"

/**
 * @brief The MidiTool application settings
 */
class MidiToolSettings : protected PhGenericSettings
{
public:
	PH_SETTING_BOOL2(setWriteMTC, writeMTC, true)
	PH_SETTING_BOOL2(setReadMTC, readMTC, true)

	PH_SETTING_INT2(setFirstFrame, firstFrame, 25 * 60 * 60)
	PH_SETTING_INT2(setLength, length, 25 * 60)

	PH_SETTING_STRING2(setMidiInputPortName, midiInputPortName, "MidiTool")
	PH_SETTING_STRING2(setMidiOutputPortName, midiOutputPortName, "MidiTool")

	PH_SETTING_INT2(setLogMask, logMask, 1)
};

#endif // MIDITOOLSETTINGS_H
