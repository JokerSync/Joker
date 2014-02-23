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
	// PhGraphicSettings :
	PH_SETTING_INT(ScreenDelay, screenDelay)

	// PhVideoSettings :
	PH_SETTING_BOOL(setVideoDeinterlace, videoDeinterlace)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 10)
};

#endif // VIDEOTESTSETTINGS_H
