#include "VideoTestSettings.h"

int VideoTestSettings::screenDelay()
{
	return intValue("screenDelay");
}

int VideoTestSettings::screenRefreshInterval()
{
	return intValue("screenRefreshIntervale", 10);
}

bool VideoTestSettings::videoDeinterlace()
{
	return boolValue("videoDeinterlace");
}

void VideoTestSettings::setLastFile(QString lastFile)
{
	setStringValue("lastFile", lastFile);
}

QString VideoTestSettings::lastFile()
{
	return stringValue("lastFile");
}
