#ifndef SONYTESTSETTINGS_H
#define SONYTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhSync/PhSyncSettings.h"

class SonyTestSettings : PhGenericSettings, public PhSyncSettings
{
public:
	// PhSyncSettings:
	bool videoSyncUp();
	unsigned char sonyDevice1();
	unsigned char sonyDevice2();
	float sonyFastRate();

	// Others settings
	bool sonyMasterActive();
	bool setSonyMasterActive(bool value);
	bool sonySlaveActive();
	bool setSonySlaveActive(bool value);
};

#endif // SONYTESTSETTINGS_H
