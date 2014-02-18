#include "SonyTestSettings.h"

bool SonyTestSettings::videoSyncUp()
{
	return boolValue("videoSyncUp", true);
}

unsigned char SonyTestSettings::sonyDevice1()
{
	return (unsigned char)intValue("sonyDevice1", 0xF0);
}

unsigned char SonyTestSettings::sonyDevice2()
{
	return (unsigned char)intValue("sonyDevice2", 0xC0);
}

float SonyTestSettings::sonyFastRate()
{
	return floatValue("sonyFastRate", 3.0f);
}

bool SonyTestSettings::sonyMasterActive()
{
	return boolValue("sonyMasterActive", true);
}

bool SonyTestSettings::setSonyMasterActive(bool value)
{
	setBoolValue("sonyMasterActive", value);
}

bool SonyTestSettings::sonySlaveActive()
{
	return boolValue("sonySlaveActive", true);
}

bool SonyTestSettings::setSonySlaveActive(bool value)
{
	setBoolValue("sonySlaveActive", value);
}
