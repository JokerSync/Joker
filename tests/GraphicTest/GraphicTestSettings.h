#ifndef GRAPHICTESTSETTINGS_H
#define GRAPHICTESTSETTINGS_H

#include <QApplication>

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhWindowSettings.h"
#include "PhGraphic/PhGraphicSettings.h"

class GraphicTestSettings : protected PhGenericSettings, public PhWindowSettings, public PhGraphicSettings
{
public:
	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhGraphicSettings
	PH_SETTING_BOOL(setDisplayInfo, displayInfo)
	PH_SETTING_BOOL(setResetInfo, resetInfo)

	// Other settings
	PH_SETTING_STRING2(setFontFile, fontFile, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/SWENSON.TTF")

	PH_SETTING_STRING2(setFont1File, font1File, QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/SWENSON.TTF")
	PH_SETTING_STRING2(setFont2File, font2File, QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/Cappella.ttf")
	PH_SETTING_INT2(setTextBoldness, textBoldness, 1)

	PH_SETTING_STRING2(setTextContent, textContent, "Change the text from the settings")

	PH_SETTING_BOOL2(setDisplayImage, displayImage, true)
	PH_SETTING_BOOL2(setDisplayRects, displayRect, true)
	PH_SETTING_BOOL2(setDisplayLoops, displayLoops, true)
	PH_SETTING_BOOL2(setDisplayArrows, displayArrows, true)
	PH_SETTING_BOOL2(setDisplayDisc, displayDisc, true)
	PH_SETTING_BOOL2(setDisplayCharacters, displayCharacters, true)
	PH_SETTING_BOOL2(setDisplayStaticText, displayStaticText, true)
	PH_SETTING_BOOL2(setDisplayMovingText, displayMovingText, true)
};

#endif // GRAPHICTESTSETTINGS_H
