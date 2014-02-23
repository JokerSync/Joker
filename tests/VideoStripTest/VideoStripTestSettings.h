#ifndef VIDEOSTRIPTESTSETTINGS_H
#define VIDEOSTRIPTESTSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"
#include "PhVideo/PhVideoSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class VideoStripTestSettings : PhGenericSettings,
	public PhGraphicStripSettings,
	public PhVideoSettings,
	public PhDocumentWindowSettings
{
public:
	PH_SETTING_INT(setScreenDelay, screenDelay)

	// PhGraphicStripSettings :
	PH_SETTING_FLOAT2(setStripHeight, stripHeight, 0.25f)
	PH_SETTING_INT2(setHorizontalSpeed, horizontalSpeed, 12)
	PH_SETTING_STRING(setTextFontFile, textFontFile)
	PH_SETTING_INT2(setTextBoldness, textBoldness, 1)
	PH_SETTING_BOOL(setStripTestMode, stripTestMode)
	PH_SETTING_BOOL2(setDisplayNextText, displayNextText, true)
	PH_SETTING_INT2(setVerticalSpeed, verticalSpeed, 1)

	// PhVideoSettings :
	PH_SETTING_BOOL(setVideoDeinterlace, videoDeinterlace)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 10)

	// Other settings :
	PH_SETTING_STRING(setLastVideoFolder, lastVideoFolder)
	PH_SETTING_BOOL(setFullScreen, fullScreen)
};

#endif // VIDEOSTRIPTESTSETTINGS_H
