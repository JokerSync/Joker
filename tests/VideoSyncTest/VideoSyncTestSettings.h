#ifndef VIDEOSYNCTESTSETTINGS_H
#define VIDEOSYNCTESTSETTINGS_H

#include <QDir>

#include "PhSync/PhSyncSettings.h"
#include "../VideoTest/VideoTestSettings.h"

class VideoSyncTestSettings : public VideoTestSettings,
	public PhSyncSettings
{
public:
	// PhSyncSettings:
	PH_SETTING_BOOL2(setVideoSyncUp, videoSyncUp, true)
	PH_SETTING_UCHAR2(setSonyDevice1, sonyDevice1, 0xF0)
	PH_SETTING_UCHAR2(setSonyDevice2, sonyDevice2, 0xC0)
	PH_SETTING_FLOAT2(setSonyFastRate, sonyFastRate, 3)
	PH_SETTING_STRING2(setSonySlavePortSuffix, sonySlavePortSuffix, "A")
	PH_SETTING_STRING2(setSonyMasterPortSuffix, sonyMasterPortSuffix, "B")
};

#endif // VIDEOSYNCTESTSETTINGS_H
