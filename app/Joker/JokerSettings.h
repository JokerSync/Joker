#ifndef JOKERSETTINGS_H
#define JOKERSETTINGS_H

#include "PhTools/PhFile.h"
#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhSyncSettings.h"
#include "PhSync/PhTimeCode.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"
#include "PhCommonUI/PhFeedbackSettings.h"
#ifdef USE_VIDEO
#include "PhVideo/PhVideoSettings.h"
#endif
#ifdef USE_SONY
#include "PhSony/PhSonySettings.h"
#endif
#ifdef USE_LTC
#include "PhLtc/PhLtcReaderSettings.h"
#endif

/**
 * @brief The Joker application settings
 */
class JokerSettings : public QObject,
	//public PhGenericSettings,
	public PhGraphicStripSettings,
	public PhDocumentWindowSettings,
	public PhFeedbackSettings,
#ifdef USE_VIDEO
	public PhVideoSettings,
#endif
#ifdef USE_SONY
	public PhSonySettings,
#endif
#ifdef USE_LTC
	public PhLtcReaderSettings,
#endif
	public PhSyncSettings
{
	Q_OBJECT

public:
	/**
	 * @brief JokerSettings constructor
	 * @param clear True if the settings shall be reset
	 */
	JokerSettings(bool clear = false);

	/**
	 * @brief Reset the settings to its default value.
	 */
	void clear();

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

	// Sync settings
	PH_SETTING_TIME(setSyncLoopTimeIn, syncLoopTimeIn)
	PH_SETTING_TIME(setSyncLoopTimeOut, syncLoopTimeOut)
	PH_SETTING_BOOL(setSyncLooping, syncLooping)

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
	PH_SETTING_STRING2(setHudFontFile, hudFontFile, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/Helvetica.ttf")
	PH_SETTING_STRING2(setTextFontFile, textFontFile, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/SWENSON.TTF")
	PH_SETTING_INT2(setTextBoldness, textBoldness, 2)
	PH_SETTING_BOOL(setStripTestMode, stripTestMode)
	PH_SETTING_BOOL2(setDisplayNextText, displayNextText, true)
	PH_SETTING_BOOL(setHideSelectedPeoples, hideSelectedPeoples)
	PH_SETTING_STRINGLIST(setSelectedPeopleNameList, selectedPeopleNameList)
	PH_SETTING_BOOL(setInvertColor, invertColor)
	PH_SETTING_BOOL2(setDisplayCuts, displayCuts, true)
	PH_SETTING_BOOL(setDisplayFeet, displayFeet)
	PH_SETTING_INT(setFirstFootTime, firstFootTime)
	PH_SETTING_INT2(setTimeBetweenTwoFeet, timeBetweenTwoFeet, 24000)
	PH_SETTING_INT(setTimePlayed, timePlayed)
	PH_SETTING_BOOL(setDisplayVerticalScale, displayVerticalScale)
	PH_SETTING_INT2(setVerticalScaleSpaceInSeconds, verticalScaleSpaceInSeconds, 5)

	PH_SETTING_INT2(setCutWidth, cutWidth, 4)
	PH_SETTING_BOOL2(setDisplayBackground, displayBackground, true)
	PH_SETTING_INT2(setBackgroundColorLight, backgroundColorLight, 0xe7dcb3)
	PH_SETTING_INT2(setBackgroundColorDark, backgroundColorDark, 0x242e2c)

#ifdef USE_VIDEO
	// Video settings:
	PH_SETTING_BOOL(setUseNativeVideoSize, useNativeVideoSize)
#endif

	// Synchronisation settings:
	PH_SETTING_INT(setSynchroProtocol, synchroProtocol)

#ifdef USE_SONY
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
#endif

#ifdef USE_LTC
	// LTC settings:
	PH_SETTING_STRING(setLtcInputPort, ltcInputPort)
	PH_SETTING_BOOL(setLtcAutoDetectTimeCodeType, ltcAutoDetectTimeCodeType)
	PH_SETTING_INT2(setLtcReaderTimeCodeType, ltcReaderTimeCodeType, PhTimeCodeType25)
#endif

#ifdef USE_MIDI
	// MTC settings:
	PH_SETTING_STRING2(setMtcInputPort, mtcInputPort, "Joker")
	PH_SETTING_STRING2(setMtcVirtualInputPort, mtcVirtualInputPort, "Joker")
	PH_SETTING_BOOL2(setMtcInputUseExistingPort, mtcInputUseExistingPort, true)

	// MMC settings
	PH_SETTING_BOOL(setSendMmcMessage, sendMmcMessage)
	PH_SETTING_STRING2(setMmcOutputPort, mmcOutputPort, "Joker")
#endif

	// PeopleDialog
	PH_SETTING_BYTEARRAY(setPeopleDialogGeometry, peopleDialogGeometry)

	// Other settings :
	PH_SETTING_STRING(setLastVideoFolder, lastVideoFolder)
	PH_SETTING_STRINGLIST2(setStripFileType, stripFileType, QStringList({"joker", "detx", "mos", "drb", "syn6"}))
	PH_SETTING_STRINGLIST2(setVideoFileType, videoFileType, QStringList({"m4v", "mkv", "avi", "mov", "mxf", "mp4"}))

	PH_SETTING_BOOL(setDisplayControlPanel, displayControlPanel)

	PH_SETTING_INT2(setLogMask, logMask, 1)

	PH_SETTING_BOOL(setUseQuarterFrame, useQuarterFrame)

	PH_SETTING_BOOL2(setDisplayLogo, displayLogo, true)

	PH_SETTING_STRING(setLanguage, language)

	PH_SETTING_BOOL(setHideStrip, hideStrip)

	PH_SETTING_INT(setLastPreferencesTab, lastPreferencesTab)

signals:
	void changed();

private:
	/**
	 * @brief Set an integer value
	 * @param name The settings name
	 * @param value The integer value
	 */
	void setIntValue(QString name, int value);
	/**
	 * @brief Get an integer value
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The integer value
	 */
	int intValue(QString name, int defaultValue = 0);
	/**
	 * @brief Set a long long value (64 bit)
	 * @param name The settings name
	 * @param value The long long value
	 */
	void setLongLongValue(QString name, qlonglong value);
	/**
	 * @brief Get a long long value (64 bit)
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The long long value
	 */
	qlonglong longLongValue(QString name, qlonglong defaultValue = 0);
	/**
	 * @brief Get an integer value with alias
	 *
	 * @param name The settings name
	 * @param alias An alias
	 * @return The integer value
	 */
	int intValueWithAlias(QString name, QString alias);

	/**
	 * @brief Set a bool value
	 * @param name The settings name
	 * @param value The bool value
	 */
	void setBoolValue(QString name, bool value);
	/**
	 * @brief Get a bool value
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The bool value
	 */
	bool boolValue(QString name, bool defaultValue = false);

	/**
	 * @brief Set a float value
	 * @param name The settings name
	 * @param value The float value
	 */
	void setFloatValue(QString name, float value);
	/**
	 * @brief Get a float value
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The float value
	 */
	float floatValue(QString name, float defaultValue = 0);
	/**
	 * @brief Set a string value
	 * @param name The settings name
	 * @param value The string value
	 */
	void setStringValue(QString name, QString value);
	/**
	 * @brief Get a string value
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The string value
	 */
	QString stringValue(QString name, QString defaultValue = "");
	/**
	 * @brief Set a string list
	 * @param name The settings name
	 * @param list The string list
	 */
	void setStringList(QString name, QStringList list);
	/**
	 * @brief Get a string list
	 * @param name The settings name
	 * @param defaultValue The default value
	 * @return The string list
	 */
	QStringList stringList(QString name, QStringList defaultValue = QStringList());

	/**
	 * @brief Set a byte array
	 * @param name The settings name
	 * @param array The byte array
	 */
	void setByteArray(QString name, QByteArray array);
	/**
	 * @brief Get a byte array
	 * @param name The settings name
	 * @return The byte array
	 */
	QByteArray byteArray(QString name);

protected:
	/**
	 * @brief The QSettings object
	 */
	QSettings _settings;
};

#endif // JOKERSETTINGS_H
