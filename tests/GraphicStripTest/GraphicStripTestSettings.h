#ifndef GRAPHICSTRIPTESTSETTINGS_H
#define GRAPHICSTRIPTESTSETTINGS_H

#include <QSettings>

#include "PhTools/PhGenericSettings.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class GraphicStripTestSettings : PhGenericSettings,
	public PhGraphicStripSettings,
	public PhDocumentWindowSettings

{
public:
	PH_SETTING_INT(setScreenDelay, screenDelay)

	// PhGraphicStripSettings :
	PH_SETTING_FLOAT2(setStripHeight, stripHeight, 0.25f)
	PH_SETTING_INT2(setHorizontalSpeed, horizontalSpeed, 12)
	PH_SETTING_STRING(setTextFontFile, textFontFile)
	PH_SETTING_INT2(setTextBoldness, textBoldness, 1)
	PH_SETTING_BOOL(setStripTestMode, stripTestMode)
	PH_SETTING_BOOL2(setDisplayNextText, displayNextText, true)
	PH_SETTING_INT2(setVerticalSpeed, verticalSpeed, 1)
	PH_SETTING_BOOL(setInvertColor, invertColor)
	PH_SETTING_BOOL(setDisplayRuler, displayRuler)
	PH_SETTING_INT(setRulerTimestamp, rulerTimestamp)
	PH_SETTING_INT2(setSpaceBetweenRuler, spaceBetweenRuler, 50)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 10)
	PH_SETTING_BOOL2(setAutoReload, autoReload, true)

	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// Other settings :
	PH_SETTING_INT2(setPeopleNumber, peopleNumber, 1)
	PH_SETTING_INT2(setLoopNumber, loopNumber, 1)
	PH_SETTING_INT2(setTextNumber, textNumber, 10)
	PH_SETTING_INT2(setTrackNumber, trackNumber, 4)
	PH_SETTING_STRING2(setTextContent, textContent, "Per hoc minui studium suum existimans Paulus.")
	PH_SETTING_INT2(setStartFrame, startFrame, 90000)
};

#endif // GRAPHICSTRIPTESTSETTINGS_H
