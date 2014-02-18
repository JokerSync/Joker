#ifndef PHGRAPHICSTRIPSETTINGS_H
#define PHGRAPHICSTRIPSETTINGS_H

#include "PhGraphic/PhGraphicSettings.h"

class PhGraphicStripSettings : public PhGraphicSettings
{
public:
	virtual float stripHeight() = 0;
	virtual int horizontalSpeed() = 0;
	virtual void setTextFontFile(QString value) = 0;
	virtual QString textFontFile() = 0;
	virtual int textBoldness() = 0;
	virtual int screenDelay() = 0;
	virtual bool stripTestMode() = 0;
	virtual bool displayTextPrediction() = 0;
	virtual bool verticalSpeed() = 0;
};

#endif // PHGRAPHICSTRIPSETTINGS_H
