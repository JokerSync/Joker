#ifndef LTCTOOLSETTINGS_H
#define LTCTOOLSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhLtc/PhLtcReaderSettings.h"
#include "PhSync/PhTimeCode.h"

/**
 * @brief The LTCTool application settings
 */
class LTCToolSettings : protected PhGenericSettings,
	public PhLtcReaderSettings
{
	Q_OBJECT

public:
	PH_SETTING_BOOL2(setGenerate, generate, true)
	PH_SETTING_BOOL2(setRead, read, true)

	PH_SETTING_INT2(setWriterTimeCodeType, writerTimeCodeType, PhTimeCodeType25)
	PH_SETTING_INT2(setWriterTimeIn, writerTimeIn, PhTimeCode::timeFromString("01:00:00:00", PhTimeCodeType25))
	PH_SETTING_INT2(setWriterLoopLength, writerLoopLength, PhTimeCode::timeFromString("00:01:00:00", PhTimeCodeType25))

	PH_SETTING_STRING(setAudioOutput, audioOutput)
	PH_SETTING_STRING(setAudioInput, audioInput)

	PH_SETTING_INT2(setLogMask, logMask, 1)

	PH_SETTING_BOOL(setLtcAutoDetectTimeCodeType, ltcAutoDetectTimeCodeType)
	PH_SETTING_STRING(setLtcInputPort, ltcInputPort)
	PH_SETTING_INT2(setLtcReaderTimeCodeType, ltcReaderTimeCodeType, PhTimeCodeType25)
};

#endif // LTCTOOLSETTINGS_H
