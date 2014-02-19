#include "VideoSyncTestSettings.h"



int VideoSyncTestSettings::screenDelay()
{
	return intValue("screenDelay");
}

int VideoSyncTestSettings::screenRefreshInterval()
{
	return intValue("screenRefreshInterval");
}

bool VideoSyncTestSettings::videoDeinterlace()
{
	return boolValue("videoDeinterlace");
}

bool VideoSyncTestSettings::videoSyncUp()
{
	return boolValue("videoSyncUp", true);
}

unsigned char VideoSyncTestSettings::sonyDevice1()
{
	return (unsigned char)intValue("sonyDevice1", 0xF0);
}

unsigned char VideoSyncTestSettings::sonyDevice2()
{
	return (unsigned char)intValue("sonyDevice2", 0xC0);
}

float VideoSyncTestSettings::sonyFastRate()
{
	return floatValue("sonyFastRate", 3.0f);
}

int VideoSyncTestSettings::logLevel()
{
	return intValue("logLevel", 1);
}

void VideoSyncTestSettings::setLastFile(QString lastFile)
{
	setStringValue("lastFile", lastFile);
}

QString VideoSyncTestSettings::lastFile()
{
	return stringValue("lastFile");
}
