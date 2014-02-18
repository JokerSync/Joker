#ifndef PHSYNCSETTINGS_H
#define PHSYNCSETTINGS_H

#include <QString>

class PhSyncSettings {
public:
	virtual bool videoSyncUp() = 0;
	virtual unsigned char sonyDevice1() = 0;
	virtual unsigned char sonyDevice2() = 0;
	virtual float sonyFastRate() = 0;
};

#endif // PHSYNCSETTINGS_H
