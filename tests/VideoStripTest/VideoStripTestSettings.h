#ifndef VIDEOSTRIPTESTSETTINGS_H
#define VIDEOSTRIPTESTSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhSyncSettings.h"
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

	// PhGraphicSettings
	PH_SETTING_BOOL(setDisplayInfo, displayInfo)
	PH_SETTING_BOOL(setResetInfo, resetInfo)

	// PhGraphicStripSettings :
	PH_SETTING_FLOAT2(setStripHeight, stripHeight, 0.25f)
	PH_SETTING_INT2(setHorizontalTimePerPixel, horizontalTimePerPixel, 80)
	PH_SETTING_INT2(setVerticalTimePerPixel, verticalTimePerPixel, 1000)
	PH_SETTING_STRING2(setBackgroundImageLight, backgroundImageLight, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240.png")
	PH_SETTING_STRING2(setBackgroundImageDark, backgroundImageDark, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240_black.png")
	PH_SETTING_STRING2(setHudFontFamily, hudFontFamily, "Helvetica")
	PH_SETTING_STRING2(setTextFontFamily, textFontFamily, "Capella")
	PH_SETTING_BOOL(setStripTestMode, stripTestMode)
	PH_SETTING_BOOL2(setDisplayNextText, displayNextText, true)
	PH_SETTING_BOOL(setHideSelectedPeoples, hideSelectedPeoples)
	PH_SETTING_BOOL(setInvertColor, invertColor)
	PH_SETTING_BOOL(setDisplayFeet, displayFeet)
	PH_SETTING_INT(setFirstFootTime, firstFootTime)
	PH_SETTING_INT2(setTimeBetweenTwoFeet, timeBetweenTwoFeet, 48000)
	PH_SETTING_BOOL2(setDisplayCuts, displayCuts, true)
	PH_SETTING_INT2(setCutWidth, cutWidth, 4)
	PH_SETTING_BOOL2(setDisplayBackground, displayBackground, true)
	PH_SETTING_INT2(setBackgroundColorLight, backgroundColorLight, 0xe7dcb3)
	PH_SETTING_INT2(setBackgroundColorDark, backgroundColorDark, 0x242e2c)
	PH_SETTING_INT2(setVerticalScaleSpaceInSeconds, verticalScaleSpaceInSeconds, 5)
	PH_SETTING_BOOL2(setDisplayVerticalScale, displayVerticalScale, false)

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

	// Other settings :
	PH_SETTING_STRING(setLastVideoFolder, lastVideoFolder)
};

#endif // VIDEOSTRIPTESTSETTINGS_H
