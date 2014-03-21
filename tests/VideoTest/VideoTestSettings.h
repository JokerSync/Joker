#ifndef VIDEOTESTSETTINGS_H
#define VIDEOTESTSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhVideo/PhVideoSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class VideoTestSettings : PhGenericSettings,
	public PhVideoSettings,
	public PhDocumentWindowSettings
{
public:
	PH_SETTING_INT(ScreenDelay, screenDelay)

	// PhVideoSettings :
	PH_SETTING_BOOL(setVideoDeinterlace, videoDeinterlace)
	PH_SETTING_INT2(setVideoBufferSize, videoBufferSize, 100)

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

#endif // VIDEOTESTSETTINGS_H
