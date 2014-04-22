#ifndef VIDEOSYNCTESTSETTINGS_H
#define VIDEOSYNCTESTSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhVideo/PhVideoSettings.h"
#include "PhSync/PhSyncSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class VideoSyncTestSettings : PhGenericSettings,
	public PhVideoSettings,
	public PhSyncSettings,
	public PhDocumentWindowSettings
{
public:
	PH_SETTING_INT(setScreenDelay, screenDelay)

	PH_SETTING_BOOL(setResetInfo, resetInfo)
	PH_SETTING_INT2(setVideoBufferSize, videoBufferSize, 100)

	// PhVideoSettings :

	// PhSyncSettings:
	PH_SETTING_BOOL2(setVideoSyncUp, videoSyncUp, true)
	PH_SETTING_UCHAR2(setSonyDevice1, sonyDevice1, 0xF0)
	PH_SETTING_UCHAR2(setSonyDevice2, sonyDevice2, 0xC0)
	PH_SETTING_FLOAT2(setSonyFastRate, sonyFastRate, 3)
	PH_SETTING_STRING2(setSonySlavePortSuffix, sonySlavePortSuffix, "A")
	PH_SETTING_STRING2(setSonyMasterPortSuffix, sonyMasterPortSuffix, "B")

	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 10)
	PH_SETTING_BOOL2(setAutoReload, autoReload, true)
};

#endif // VIDEOSYNCTESTSETTINGS_H
