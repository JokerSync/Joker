/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef GRAPHICSTRIPTESTSETTINGS_H
#define GRAPHICSTRIPTESTSETTINGS_H

#include <QDir>
#include "PhGraphicStrip/PhGraphicStripSettings.h"

class GraphicStripTestSettings : public PhGraphicStripSettings
{
public:
	// PhGraphicSettings
	int screenDelay() {
		return 0;
	}
	bool displayInfo() {
		return false;
	}
	bool resetInfo() {
		return false;
	}

	// PhGraphicStripSettings :
	float stripHeight() {
		return 1;
	}
	int horizontalTimePerPixel() {
		return 80;
	}
	int verticalTimePerPixel() {
		return 1000;
	}
	QString textFontFile() {
		return QCoreApplication::applicationDirPath() + "/SWENSON.TTF";
	}
	int textBoldness() {
		return 1;
	}
	bool stripTestMode() {
		return false;
	}
	bool displayNextText() {
		return true;
	}
	bool invertColor() {
		return false;
	}
	bool displayRuler() {
		return false;
	}
	int rulerTimeIn() {
		return 0;
	}
	int timeBetweenRuler() {
		return 48000;
	}
};

#endif // GRAPHICSTRIPTESTSETTINGS_H
