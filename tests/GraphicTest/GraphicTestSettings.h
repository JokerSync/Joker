#ifndef GRAPHICTESTSETTINGS_H
#define GRAPHICTESTSETTINGS_H

#include <QApplication>

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhWindowSettings.h"
#include "PhGraphic/PhGraphicSettings.h"

class GraphicTestSettings : protected PhGenericSettings, public PhWindowSettings, public PhGraphicSettings
{
	// PhGraphicSettings
	PH_SETTING_BOOL(setDisplayInfo, displayInfo)
	PH_SETTING_STRING2(setInfoFontFile, infoFontFile, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/ARIAL.TTF")
	PH_SETTING_STRING2(setFontFile, fontFile, QApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/SWENSON.TTF")
};

#endif // GRAPHICTESTSETTINGS_H
