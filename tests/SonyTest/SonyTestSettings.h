#ifndef SONYTESTSETTINGS_H
#define SONYTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhSyncSettings.h"

class SonyTestSettings : PhGenericSettings, public PhSyncSettings
{
public:
	// PhSyncSettings:
	PH_SETTING_BOOL2(setVideoSyncUp, videoSyncUp, true)
	PH_SETTING_UCHAR2(setSonyDevice1, sonyDevice1, 0xF0)
	PH_SETTING_UCHAR2(setSonyDevice2, sonyDevice2, 0xC0)
	PH_SETTING_FLOAT2(setSonyFastRate, sonyFastRate, 3)
	PH_SETTING_STRING2(setSlavePortDescription, slavePortDescription, "Dual RS232 A")
	PH_SETTING_STRING2(setMasterPortDescription, masterPortDescription, "Dual RS232 B")

	// Others settings
	PH_SETTING_BOOL2(setSonySlaveActive, sonySlaveActive, true);
	PH_SETTING_BOOL2(setSonyMasterActive, sonyMasterActive, true)

	PH_SETTING_BOOL2(setUseVideoSlaveSync, useVideoSlaveSync, true);
	PH_SETTING_BOOL2(setUseVideoMasterSync, useVideoMasterSync, true);

};

#endif // SONYTESTSETTINGS_H
