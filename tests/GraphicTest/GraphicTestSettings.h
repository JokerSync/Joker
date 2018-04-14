#ifndef GRAPHICTESTSETTINGS_H
#define GRAPHICTESTSETTINGS_H

#include <QApplication>

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhWindowSettings.h"
#include "PhGraphic/PhGraphicSettings.h"

class GraphicTestSettings : public PhGenericSettings, public PhWindowSettings, public PhGraphicSettings
{
	Q_OBJECT

public:
	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhGraphicSettings
	PH_SETTING_BOOL(setDisplayInfo, displayInfo)
	PH_SETTING_BOOL(setResetInfo, resetInfo)

	// Other settings
	PH_SETTING_STRING2(setFont1Family, font1Family, "SWENSON")
	PH_SETTING_STRING2(setFont2Family, font2Family, "Cappella")
	PH_SETTING_INT2(setTextWeight, textWeight, 400)

	PH_SETTING_STRING2(setTextContent, textContent, "Change the text from the settings")

	PH_SETTING_BOOL2(setDisplayImage1, displayImage1, true)
	PH_SETTING_BOOL2(setDisplayImage2, displayImage2, true)
	PH_SETTING_BOOL2(setDisplayRects, displayRect, true)
	PH_SETTING_BOOL2(setDisplayArrows, displayArrows, true)
	PH_SETTING_BOOL2(setDisplayDisc, displayDisc, true)
	PH_SETTING_BOOL2(setDisplayCharacters, displayCharacters, true)
	PH_SETTING_BOOL2(setDisplayStaticText, displayStaticText, true)
	PH_SETTING_BOOL2(setDisplayMovingText, displayMovingText, true)
};

#endif // GRAPHICTESTSETTINGS_H
