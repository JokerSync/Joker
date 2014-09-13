#ifndef JOKERSETTINGS_H
#define JOKERSETTINGS_H

#include <QDir>
#include <QApplication>

#include "PhTools/PhGenericSettings.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"
#include "PhVideo/PhVideoSettings.h"
#include "PhSony/PhSonySettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"
#include "PhCommonUI/PhFeedbackSettings.h"
#include "PhLtc/PhLtcReaderSettings.h"


/**
 * @brief The Joker application settings
 */
class JokerSettings : public PhGenericSettings,
	public PhGraphicStripSettings,
	public PhVideoSettings,
	public PhSonySettings,
	public PhDocumentWindowSettings,
	public PhFeedbackSettings,
	public PhLtcReaderSettings
{
public:
	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BOOL2(setExitedNormaly, exitedNormaly, true)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 10)
	PH_SETTING_BOOL2(setAutoReload, autoReload, true)

	// PhFeedbackSettings
	QString settingsFileName() {
		return _settings.fileName();
	}
	PH_SETTING_STRINGLIST(setEmailList, emailList)


	PH_SETTING_INT3(setScreenDelay, screenDelay, delay)

	// PhGraphicSettings
	PH_SETTING_BOOL(setDisplayInfo, displayInfo)
	PH_SETTING_STRING2(setFontFile, fontFile, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/SWENSON.TTF")
	PH_SETTING_BOOL(setResetInfo, resetInfo)

	// PhGraphicStripSettings :
	PH_SETTING_FLOAT2(setStripHeight, stripHeight, 0.25f)
	PH_SETTING_INT2(setHorizontalTimePerPixel, horizontalTimePerPixel, 50)
	PH_SETTING_INT2(setVerticalTimePerPixel, verticalTimePerPixel, 1000)
	PH_SETTING_STRING2(setBackgroundImageLight, backgroundImageLight, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240.png")
	PH_SETTING_STRING2(setBackgroundImageDark, backgroundImageDark, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240_black.png")
	PH_SETTING_STRING2(setHudFontFile, hudFontFile, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/HelveticaCYPlain.ttf")
	PH_SETTING_STRING2(setTextFontFile, textFontFile, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/SWENSON.TTF")
	PH_SETTING_INT2(setTextBoldness, textBoldness, 2)
	PH_SETTING_BOOL(setStripTestMode, stripTestMode)
	PH_SETTING_BOOL2(setDisplayNextText, displayNextText, true)
	PH_SETTING_STRINGLIST(setSelectedPeopleNameList, selectedPeopleNameList)
	PH_SETTING_BOOL(setInvertColor, invertColor)
	PH_SETTING_BOOL(setDisplayRuler, displayRuler)
	PH_SETTING_INT(setRulerTimeIn, rulerTimeIn)
	PH_SETTING_INT2(setTimeBetweenRuler, timeBetweenRuler, 24000)
	PH_SETTING_INT(setTimePlayed, timePlayed)
	PH_SETTING_INT2(setVerticalScaleSpaceInSeconds, verticalScaleSpaceInSeconds, 5)
	PH_SETTING_BOOL(setDisplayVerticalScale, displayVerticalScale)

	PH_SETTING_INT2(setCutWidth, cutWidth, 4)
	PH_SETTING_BOOL2(setDisplayBackground, displayBackground, true)
	PH_SETTING_INT2(setBackgroundColorLight, backgroundColorLight, 0xe7dcb3)
	PH_SETTING_INT2(setBackgroundColorDark, backgroundColorDark, 0x242e2c)

	// Synchronisation settings:
	PH_SETTING_INT(setSynchroProtocol, synchroProtocol)

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

	// LTC settings:
	PH_SETTING_STRING(setLtcInputPort, ltcInputPort)
	PH_SETTING_BOOL(setLtcAutoDetectTimeCodeType, ltcAutoDetectTimeCodeType)
	PH_SETTING_INT2(setLtcReaderTimeCodeType, ltcReaderTimeCodeType, PhTimeCodeType25)

	// MTC settings:
	PH_SETTING_STRING2(setMtcInputPort, mtcInputPort, "Joker")
	PH_SETTING_STRING2(setMtcVirtualInputPort, mtcVirtualInputPort, "Joker")

	// MMC settings
	PH_SETTING_BOOL(setSendMmcMessage, sendMmcMessage)
	PH_SETTING_STRING2(setMmcOutputPort, mmcOutputPort, "Joker")

	// PeopleDialog
	PH_SETTING_BYTEARRAY(setPeopleDialogGeometry, peopleDialogGeometry)

	// Other settings :
	PH_SETTING_STRING(setLastVideoFolder, lastVideoFolder)
	PH_SETTING_STRINGLIST2(setStripFileType, stripFileType, QStringList({"joker", "detx", "mos", "drb", "syn6"}))
	PH_SETTING_STRINGLIST2(setVideoFileType, videoFileType, QStringList({"m4v", "mkv", "avi", "mov", "mxf"}))


	PH_SETTING_INT2(setLogMask, logMask, 1)

	PH_SETTING_BOOL2(setDisplayTitle, displayTitle, true)
	PH_SETTING_BOOL2(setDisplayNextTC, displayNextTC, true)
	PH_SETTING_BOOL2(setDisplayTC, displayTC, true)
	PH_SETTING_BOOL2(setDisplayLoop, displayLoop, true)
	PH_SETTING_BOOL2(setDisplayCuts, displayCuts, true)

	PH_SETTING_BOOL(setUseQuarterFrame, useQuarterFrame)

	PH_SETTING_BOOL2(setDisplayLogo, displayLogo, true)

	PH_SETTING_STRING(setLanguage, language)

	PH_SETTING_BOOL(setHideStrip, hideStrip)

	PH_SETTING_INT(setLastPreferencesTab, lastPreferencesTab)
};

#endif // JOKERSETTINGS_H
