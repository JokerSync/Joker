#ifndef JOKERSETTINGS_H
#define JOKERSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhGraphic/PhGraphicSettings.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"
#include "PhVideo/PhVideoSettings.h"
#include "PhSync/PhSyncSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class JokerSettings : PhGenericSettings,
	public PhGraphicSettings,
	public PhGraphicStripSettings,
	public PhVideoSettings,
	public PhSyncSettings,
	public PhDocumentWindowSettings
{
public:
	// PhGraphicSettings :
	void setScreenDelay(int delay);
	int screenDelay();
	int screenRefreshInterval();

	// PhGraphicStripSettings :
	void setStripHeight(float stripHeight);
	float stripHeight();
	void setHorizontalSpeed(int speed);
	int horizontalSpeed();
	void setTextFontFile(QString value);
	QString textFontFile();
	void setTextBoldness(int boldness);
	int textBoldness();

	void setStripTestMode(bool testMode);
	bool stripTestMode();

	bool displayTextPrediction();
	bool verticalSpeed();

	// PhVideoSettings :
	void setVideoDeinterlace(bool deinterlace);
	bool videoDeinterlace();

	// PhSyncSettings:
	bool videoSyncUp();
	unsigned char sonyDevice1();
	unsigned char sonyDevice2();
	float sonyFastRate();

	// Other settings :
#warning TODO remove
	void setOpenLastFile(bool openLastFile);
	bool openLastFile();

	PH_SETTING_STRING(CurrentDocument, currentDocument)
	PH_SETTING_STRING2(LastDocumentFolder, lastDocumentFolder, QDir::homePath())

	void setLastVideoFolder(QString lastFolder);
	QString lastVideoFolder();

	void setSelectedFilter(QString filter);
	QString selectedFilter();

	PH_SETTING_STRINGLIST(RecentDocumentList, recentDocumentList)

	PH_SETTING_INT2(MaxRecentDocument, maxRecentDocument, 10)

	void setFullScreen(bool fullScreen);
	bool fullScreen();

	void setSynchroProtocol(int syncProtocol);
	int synchroProtocol();

	void setLTCInputDevice(QString ltcInputDevice);
	QString ltcInputDevice();

	void setLogMask(int logMask);
	int logMask();

	void setDisplayTitle(bool displayTitle);
	bool displayTitle();

	void setDisplayNextTC(bool displayNextTC);
	bool displayNextTC();

	void setDisplayTC(bool displayTC);
	bool displayTC();

	void setDisplayNextText(bool displayNextText);
	bool displayNextText();

	void setDisplayLoop(bool displayLoop);
	bool displayLoop();

	void setUseQuarterFrame(bool useQuarterFrame);
	bool useQuarterFrame();
};

#endif // JOKERSETTINGS_H
