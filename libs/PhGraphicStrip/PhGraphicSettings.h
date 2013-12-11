#ifndef PHGRAPHICSETTINGS_H
#define PHGRAPHICSETTINGS_H

#include <QString>

class PhGraphicSettings
{
public:
	PhGraphicSettings();

	virtual float stripHeight() = 0;
	virtual int horizontalSpeed() = 0;
	virtual QString textFontFile() = 0;
	virtual int textBoldness() = 0;
	virtual int screenDelay() = 0;
	virtual bool stripTestMode() = 0;
	virtual bool displayTextPrediction() = 0;
	virtual bool verticalSpeed() = 0;
};

#endif // PHGRAPHICSETTINGS_H
