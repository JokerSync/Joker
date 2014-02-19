#ifndef VIDEOSYNCTESTSETTINGS_H
#define VIDEOSYNCTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhVideo/PhVideoSettings.h"
#include "PhSync/PhSyncSettings.h"

class VideoSyncTestSettings : PhGenericSettings, public PhVideoSettings, public PhSyncSettings
{
public:
	// PhGraphicSettings :
	int screenDelay();
	int screenRefreshInterval();

	// PhGraphicStripSettings :
	bool videoDeinterlace();

	// PhSyncSettings:
	bool videoSyncUp();
	unsigned char sonyDevice1();
	unsigned char sonyDevice2();
	float sonyFastRate();

	// Others settings
	int logLevel();
	void setLastFile(QString lastFile);
	QString lastFile();
};

#endif // VIDEOSYNCTESTSETTINGS_H
