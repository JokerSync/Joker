#include <QDir>

#include "JokerSettings.h"

void JokerSettings::setScreenDelay(int delay)
{
	setIntValue("screenDelay", delay);
}

int JokerSettings::screenDelay()
{
	return intValue("screenDelay");
}

int JokerSettings::screenRefreshInterval()
{
	return intValue("screenRefreshInterval", 5);
}

void JokerSettings::setStripHeight(float stripHeight)
{
	setFloatValue("stripHeight", stripHeight);
}

float JokerSettings::stripHeight()
{
	return floatValue("stripHeight", 0.25f);
}

void JokerSettings::setHorizontalSpeed(int speed)
{
	setIntValue("horizontalSpeed", speed);
}

int JokerSettings::horizontalSpeed()
{
	return intValue("horizontalSpeed", 12);
}

void JokerSettings::setTextFontFile(QString value)
{
	setStringValue("textFontFile", value);
}

QString JokerSettings::textFontFile()
{
	return stringValue("textFontFile");
}

void JokerSettings::setTextBoldness(int boldness)
{
	setIntValue("textBoldness", boldness);
}

int JokerSettings::textBoldness()
{
	return intValue("textBoldness", 1);
}

void JokerSettings::setStripTestMode(bool testMode)
{
	setBoolValue("stripTestMode", testMode);
}

bool JokerSettings::stripTestMode()
{
	return boolValue("stripTestMode");
}

bool JokerSettings::displayTextPrediction()
{
	return boolValue("displayTextPrediction", true);
}

bool JokerSettings::verticalSpeed()
{
	return intValue("verticalSpeed", 6);
}

void JokerSettings::setVideoDeinterlace(bool deinterlace)
{
	setBoolValue("videoDeinterlace", deinterlace);
}

bool JokerSettings::videoDeinterlace()
{
	return boolValue("videoDeinterlace");
}

void JokerSettings::setLastVideoFolder(QString lastFolder)
{
	setStringValue("lastVideoFolder", lastFolder);
}

QString JokerSettings::lastVideoFolder()
{
	return stringValue("lastVideoFolder", QDir::homePath());
}

void JokerSettings::setSelectedFilter(QString filter)
{
	setStringValue("selectedFilter", filter);
}

QString JokerSettings::selectedFilter()
{
	return stringValue("selectedFilter", "Rythmo files (*.detx *.strip)");
}

void JokerSettings::setFullScreen(bool fullScreen)
{
	setBoolValue("fullScreen", fullScreen);
}

bool JokerSettings::fullScreen()
{
	return boolValue("fullScreen", false);
}

void JokerSettings::setSynchroProtocol(int syncProtocol)
{
	setIntValue("synchroProtocol", syncProtocol);
}

int JokerSettings::synchroProtocol()
{
	return intValue("synchroProtocol");
}

void JokerSettings::setLTCInputDevice(QString ltcInputDevice)
{
	setStringValue("ltcInputDevice", ltcInputDevice);
}

QString JokerSettings::ltcInputDevice()
{
	return stringValue("ltcInputDevice", "");
}

void JokerSettings::setLogMask(int logMask)
{
	setIntValue("logMask", logMask);
}

int JokerSettings::logMask()
{
	return intValue("logMask", 1);
}

void JokerSettings::setDisplayTitle(bool displayTitle)
{
	setBoolValue("displayTitle", displayTitle);
}

bool JokerSettings::displayTitle()
{
	return boolValue("displayTitle", true);
}

void JokerSettings::setDisplayTC(bool displayTC)
{
	setBoolValue("displayTC", displayTC);
}

bool JokerSettings::displayTC()
{
	return boolValue("displayTC", true);
}

void JokerSettings::setDisplayNextText(bool displayNextText)
{
	setBoolValue("displayNextText", displayNextText);
}

bool JokerSettings::displayNextText()
{
	return boolValue("displayNextText", true);
}

void JokerSettings::setDisplayLoop(bool displayLoop)
{
	setBoolValue("displayLoop", displayLoop);
}

bool JokerSettings::displayLoop()
{
	return boolValue("displayLoop", true);
}

void JokerSettings::setUseQuarterFrame(bool useQuarterFrame)
{
	setBoolValue("useQuarterFrame", useQuarterFrame);
}

bool JokerSettings::useQuarterFrame()
{
	return boolValue("useQuarterFrame");
}

void JokerSettings::setDisplayNextTC(bool displayNextTC)
{
	setBoolValue("displayNextTC", displayNextTC);
}

bool JokerSettings::displayNextTC()
{
	return boolValue("displayNextTC", true);
}

bool JokerSettings::videoSyncUp()
{
	return boolValue("videoSyncUp", true);
}

unsigned char JokerSettings::sonyDevice1()
{
	return (unsigned char)intValue("sonyDevice1", 0xF0);
}

unsigned char JokerSettings::sonyDevice2()
{
	return (unsigned char)intValue("sonyDevice2", 0xC0);
}

float JokerSettings::sonyFastRate()
{
	return floatValue("sonyFastRate", 3.0f);
}

void JokerSettings::setOpenLastFile(bool openLastFile)
{
	setBoolValue("openLastFile", openLastFile);
}

bool JokerSettings::openLastFile()
{
	return boolValue("openLastFile", true);
}
