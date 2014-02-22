#ifndef JOKERSETTINGS_H
#define JOKERSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhGraphic/PhGraphicSettings.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"
#include "PhVideo/PhVideoSettings.h"
#include "PhSync/PhSyncSettings.h"

class JokerSettings : PhGenericSettings,
		public PhGraphicSettings,
		public PhGraphicStripSettings,
		public PhVideoSettings,
		public PhSyncSettings
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
	void setOpenLastFile(bool openLastFile);
	bool openLastFile();

	void setLastFile(QString lastFile);
	QString lastFile();

	void setLastFolder(QString lastFolder);
	QString lastFolder();

	void setLastVideoFolder(QString lastFolder);
	QString lastVideoFolder();

	void setSelectedFilter(QString filter);
	QString selectedFilter();

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
