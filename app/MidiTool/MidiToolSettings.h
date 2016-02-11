#ifndef MIDITOOLSETTINGS_H
#define MIDITOOLSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhTimeCode.h"

/**
 * @brief The MidiTool application settings
 */
class MidiToolSettings : protected PhGenericSettings
{
public:
	PH_SETTING_BOOL2(setWriteMTC, writeMTC, true)
	PH_SETTING_BOOL2(setReadMTC, readMTC, true)

	PH_SETTING_INT2(setWriterTimeCodeType, writerTimeCodeType, PhTimeCodeType25)
	PH_SETTING_FLOAT(setWriterRate, writerRate)
	PH_SETTING_INT2(setWriterTimeIn, writerTimeIn, PhTimeCode::timeFromString("01:00:00:00", PhTimeCodeType25))
	PH_SETTING_INT2(setWriterLoopLength, writerLoopLength, PhTimeCode::timeFromString("00:01:00:00", PhTimeCodeType25))

	PH_SETTING_BOOL(setLooping, looping)

	PH_SETTING_STRING2(setMidiOutputPortName, midiOutputPortName, "MidiTool")
	PH_SETTING_STRING2(setMidiInputPortName, midiInputPortName, "MidiTool")
	PH_SETTING_STRING2(setMidiVirtualInputPortName, midiVirtualInputPortName, "MidiTool")
	PH_SETTING_BOOL2(setMidiInputUseExistingPort, midiInputUseExistingPort, true)
	PH_SETTING_BOOL(setMidiForce24as2398, midiForce24as2398)

	PH_SETTING_INT2(setLogMask, logMask, 1)
};

#endif // MIDITOOLSETTINGS_H
