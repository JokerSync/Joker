/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef GRAPHICSTRIPSPECSETTINGS_H
#define GRAPHICSTRIPSPECSETTINGS_H

#include "PhGraphicStrip/PhGraphicStripSettings.h"

class GraphicStripSpecSettings : public PhGraphicStripSettings
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

	QString backgroundImageLight() {
		return "motif-240.png";
	}

	QString backgroundImageDark() {
		return "motif-240_black.png";
	}

	QString hudFontFile() {
		return "Helvetica.ttf";
	}

	QString textFontFile() {
		return "Cappella.ttf";
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
	bool hideSelectedPeoples() {
		return false;
	}

	bool invertColor() {
		return false;
	}
	bool displayFeet() {
		return false;
	}
	int firstFootTime() {
		return 0;
	}
	int timeBetweenTwoFeet() {
		return 48000;
	}

	bool displayCuts() {
		return true;
	}

	int cutWidth() {
		return 2;
	}

	bool displayBackground() {
		return true;
	}

	int backgroundColorLight() {
		return 0xe7dcb3;
	}

	int backgroundColorDark() {
		return 0x242e2c;
	}

	bool displayVerticalScale() {
		return false;
	}

	int verticalScaleSpaceInSeconds() {
		return 5;
	}
};

#endif // GRAPHICSTRIPSPECSETTINGS_H
