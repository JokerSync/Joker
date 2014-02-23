#ifndef GRAPHICSTRIPSYNCTESTSETTINGS_H
#define GRAPHICSTRIPSYNCTESTSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"
#include "PhSync/PhSyncSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class GraphicStripSyncTestSettings : protected PhGenericSettings,
	public PhGraphicStripSettings,
	public PhSyncSettings,
	public PhDocumentWindowSettings
{
public:
	PH_SETTING_INT(setScreenDelay, screenDelay)

	// PhGraphicStripSettings :
	PH_SETTING_FLOAT2(setStripHeight, stripHeight, 0.25f)
	PH_SETTING_INT2(setHorizontalSpeed, horizontalSpeed, 12)
	PH_SETTING_STRING(setTextFontFile, textFontFile)
	PH_SETTING_INT2(setTextBoldness, textBoldness, 2)
	PH_SETTING_BOOL(setStripTestMode, stripTestMode)
	PH_SETTING_BOOL2(setDisplayNextText, displayNextText, true)
	PH_SETTING_INT2(setVerticalSpeed, verticalSpeed, 1)

	// PhSyncSettings:
	PH_SETTING_BOOL2(setVideoSyncUp, videoSyncUp, true)
	PH_SETTING_INT2(setSonyDevice1, sonyDevice1, 0xF0)
	PH_SETTING_INT2(setSonyDevice2, sonyDevice2, 0xC0)
	PH_SETTING_FLOAT2(setSonyFastRate, sonyFastRate, 3)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 10)

	// Others
	PH_SETTING_BOOL(setUseQuarterFrame, useQuarterFrame)
};

#endif // GRAPHICSTRIPSYNCTESTSETTINGS_H
