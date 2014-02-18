#include "GraphicStripTestSettings.h"

GraphicStripTestSettings::GraphicStripTestSettings()
{
}

float GraphicStripTestSettings::stripHeight()
{
	return floatValue("stripHeight", 0.25f);
}

int GraphicStripTestSettings::horizontalSpeed()
{
	return intValue("horizontalSpeed", 12);
}

void GraphicStripTestSettings::setTextFontFile(QString value)
{
	setStringValue("textFontFile", value);
}

QString GraphicStripTestSettings::textFontFile()
{
	return stringValue("textFontFile");
}

int GraphicStripTestSettings::textBoldness()
{
	return intValue("textBoldness", 1);
}

int GraphicStripTestSettings::screenDelay()
{
	return intValue("screenDelay");
}

bool GraphicStripTestSettings::stripTestMode()
{
	return boolValue("stripTestMode");
}

bool GraphicStripTestSettings::displayTextPrediction()
{
	return boolValue("displayTextPrediction", true);
}

bool GraphicStripTestSettings::verticalSpeed()
{
	return intValue("verticalSpeed", 6);
}

void GraphicStripTestSettings::setLastFile(QString lastFile)
{
	setStringValue("lastFile", lastFile);
}

QString GraphicStripTestSettings::lastFile()
{
	return stringValue("lastFile");
}

void GraphicStripTestSettings::setPeopleNumber(int number)
{
	setIntValue("peopleNumber", number);
}

int GraphicStripTestSettings::peopleNumber()
{
	return intValue("peopleNumber");
}

void GraphicStripTestSettings::setLoopNumber(int number)
{
	setIntValue("loopNumber", number);
}

int GraphicStripTestSettings::loopNumber()
{
	return intValue("loopNumber");
}

void GraphicStripTestSettings::setTextNumber(int number)
{
	setIntValue("textNumber", number);
}

int GraphicStripTestSettings::textNumber()
{
	return intValue("textNumber");
}

void GraphicStripTestSettings::setTrackNumber(int number)
{
	setIntValue("trackNumber", number);
}

int GraphicStripTestSettings::trackNumber()
{
	return intValue("trackNumber", 4);
}

void GraphicStripTestSettings::setTextContent(QString content)
{
	setStringValue("textContent", content);
}

QString GraphicStripTestSettings::textContent()
{
	return stringValue("textContent", "Per hoc minui studium suum existimans Paulus.");
}

void GraphicStripTestSettings::setStartFrame(int frame)
{
	setIntValue("startFrame", frame);
}

int GraphicStripTestSettings::startFrame()
{
	return intValue("startFrame", 90000);
}
