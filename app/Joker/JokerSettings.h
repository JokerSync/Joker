#ifndef JOKERSETTINGS_H
#define JOKERSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"
#include "PhVideo/PhVideoSettings.h"
#include "PhSync/PhSyncSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

/**
 * @brief The Joker application settings
 */
class JokerSettings : PhGenericSettings,
	public PhGraphicStripSettings,
	public PhVideoSettings,
	public PhSyncSettings,
	public PhDocumentWindowSettings
{
public:
	PH_SETTING_INT3(setScreenDelay, screenDelay, delay)

	// PhGraphicStripSettings :
	PH_SETTING_FLOAT2(setStripHeight, stripHeight, 0.25f)
	PH_SETTING_INT2(setHorizontalSpeed, horizontalSpeed, 12)
	PH_SETTING_STRING(setTextFontFile, textFontFile)
	PH_SETTING_INT2(setTextBoldness, textBoldness, 2)
	PH_SETTING_BOOL(setStripTestMode, stripTestMode)
	PH_SETTING_BOOL2(setDisplayNextText, displayNextText, true)
	PH_SETTING_INT2(setVerticalSpeed, verticalSpeed, 1)
	PH_SETTING_STRINGLIST(setSelectedPeopleNameList, selectedPeopleNameList)
	PH_SETTING_BOOL(setDisplayRuler, displayRuler)
	PH_SETTING_INT(setRulerTimestamp, rulerTimestamp)
	PH_SETTING_INT2(setSpaceBetweenRuler, spaceBetweenRuler, 50)

	// PhVideoSettings :
	PH_SETTING_BOOL(setVideoDeinterlace, videoDeinterlace)

	// PhSyncSettings:
	PH_SETTING_BOOL2(setVideoSyncUp, videoSyncUp, true)
	PH_SETTING_UCHAR2(setSonyDevice1, sonyDevice1, 0xF0)
	PH_SETTING_UCHAR2(setSonyDevice2, sonyDevice2, 0xC0)
	PH_SETTING_FLOAT2(setSonyFastRate, sonyFastRate, 3)

	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 10)

	// Other settings :
	PH_SETTING_STRING(setLastVideoFolder, lastVideoFolder)
	PH_SETTING_STRING2(setSelectedFilter, selectedFilter, "Rythmo files (*.detx *.strip)")

	PH_SETTING_INT(setSynchroProtocol, synchroProtocol)
	PH_SETTING_STRING(setLTCInputDevice, ltcInputDevice)

	PH_SETTING_INT2(setLogMask, logMask, 1)

	PH_SETTING_BOOL2(setDisplayTitle, displayTitle, true)
	PH_SETTING_BOOL2(setDisplayNextTC, displayNextTC, true)
	PH_SETTING_BOOL2(setDisplayTC, displayTC, true)
	PH_SETTING_BOOL2(setDisplayLoop, displayLoop, true)

	PH_SETTING_BOOL(setUseQuarterFrame, useQuarterFrame)

	PH_SETTING_STRING2(setVideoFileFilter, videoFileFilter, " (*.m4v *.mkv *.avi *.mov)")
};

#endif // JOKERSETTINGS_H
