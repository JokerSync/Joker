#ifndef VIDEOSYNCTESTSETTINGS_H
#define VIDEOSYNCTESTSETTINGS_H

#include <QDir>

#include "PhSync/PhTimeCode.h"
#include "PhSony/PhSonySettings.h"
#include "../VideoTest/VideoTestSettings.h"

class VideoSyncTestSettings : public VideoTestSettings,
	public PhSonySettings
{
public:
	// PhSonySettings:
	PH_SETTING_BOOL2(setVideoSyncUp, videoSyncUp, true)
	PH_SETTING_UCHAR2(setSonyDevice1, sonyDevice1, 0xF0)
	PH_SETTING_UCHAR2(setSonyDevice2, sonyDevice2, 0xC0)
	PH_SETTING_FLOAT2(setSonyFastRate, sonyFastRate, 3)
	PH_SETTING_STRING2(setSonySlavePortSuffix, sonySlavePortSuffix, "A")
	PH_SETTING_STRING2(setSonyMasterPortSuffix, sonyMasterPortSuffix, "B")

	PH_SETTING_INT2(setSonyMasterCommunicationTimeCodeType, sonyMasterCommunicationTimeCodeType, PhTimeCodeType25)
	PH_SETTING_INT2(setSonySlaveCommunicationTimeCodeType, sonySlaveCommunicationTimeCodeType, PhTimeCodeType25)

	PH_SETTING_INT2(setSonyMasterVideoSyncTimeCodeType, sonyMasterVideoSyncTimeCodeType, PhTimeCodeType25)
	PH_SETTING_INT2(setSonySlaveVideoSyncTimeCodeType, sonySlaveVideoSyncTimeCodeType, PhTimeCodeType25)
};

#endif // VIDEOSYNCTESTSETTINGS_H
