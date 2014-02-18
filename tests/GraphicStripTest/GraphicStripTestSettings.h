#ifndef GRAPHICSTRIPTESTSETTINGS_H
#define GRAPHICSTRIPTESTSETTINGS_H

#include <QSettings>

#include "PhTools/PhGenericSettings.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"

class GraphicStripTestSettings : PhGenericSettings, public PhGraphicStripSettings
{
public:
	// PhGraphicSettings :
	int screenDelay();

	// PhGraphicStripSettings :
	float stripHeight();
	int horizontalSpeed();
	void setTextFontFile(QString value);
	QString textFontFile();
	int textBoldness();
	bool stripTestMode();
	bool displayTextPrediction();
	bool verticalSpeed();

	// Other settings :
	void setLastFile(QString lastFile);
	QString lastFile();
	void setPeopleNumber(int number);
	int peopleNumber();
	void setLoopNumber(int number);
	int loopNumber();
	void setTextNumber(int number);
	int textNumber();
	void setTrackNumber(int number);
	int trackNumber();
	void setTextContent(QString content);
	QString textContent();
	void setStartFrame(int frame);
	int startFrame();
};

#endif // GRAPHICSTRIPTESTSETTINGS_H
