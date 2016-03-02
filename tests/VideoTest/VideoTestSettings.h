#ifndef VIDEOTESTSETTINGS_H
#define VIDEOTESTSETTINGS_H

#include <QApplication>
#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhSyncSettings.h"
#include "PhGraphic/PhGraphicSettings.h"
#include "PhVideo/PhVideoSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class VideoTestSettings : protected PhGenericSettings,
	public PhGraphicSettings,
	public PhVideoSettings,
	public PhDocumentWindowSettings
{
public:
	PH_SETTING_INT(setScreenDelay, screenDelay)
	PH_SETTING_INT2(setLogMask, logMask, 1)
	PH_SETTING_BOOL2(setDisplayMediaPanel, displayMediaPanel, true)
	PH_SETTING_BOOL(setDeinterlaceVideo, deinterlaceVideo)
	PH_SETTING_INT(setTimeStamp, timeStamp)
	PH_SETTING_INT(setCurrentTime, currentTime)

	// PhGraphicSettings
	PH_SETTING_BOOL(setDisplayInfo, displayInfo)
	PH_SETTING_BOOL(setResetInfo, resetInfo)

	// PhVideoSettings :
	PH_SETTING_BOOL(setUseNativeVideoSize, useNativeVideoSize)
	PH_SETTING_INT2(setVideoReadhead, videoReadhead, 50)
	PH_SETTING_FRAME2(setVideoPoolSize, videoPoolSize, 120)

	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 10)
	PH_SETTING_BOOL2(setAutoReload, autoReload, true)
	PH_SETTING_HASH(setBookmark, bookmark)
};

#endif // VIDEOTESTSETTINGS_H
