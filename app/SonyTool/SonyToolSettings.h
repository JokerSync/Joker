#ifndef SONYTESTSETTINGS_H
#define SONYTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhTimeCode.h"
#include "PhSony/PhSonySettings.h"

/**
 * @brief The SonyTool application settings
 */
class SonyToolSettings : public QObject, PhGenericSettings, public PhSonySettings
{
	Q_OBJECT

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

	// Others settings
	PH_SETTING_BOOL2(setSonySlaveActive, sonySlaveActive, true)
	PH_SETTING_BOOL2(setSonyMasterActive, sonyMasterActive, true)

	PH_SETTING_BOOL2(setUseVideoSlaveSync, useVideoSlaveSync, true)
	PH_SETTING_BOOL2(setUseVideoMasterSync, useVideoMasterSync, true)

};

#endif // SONYTESTSETTINGS_H
