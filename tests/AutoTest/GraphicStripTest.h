/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef GRAPHICSTRIPTEST_H
#define GRAPHICSTRIPTEST_H

#include <QObject>

class GraphicStripTest : public QObject
{
	Q_OBJECT
public:
	explicit GraphicStripTest(bool travis, QObject *parent = 0);

private slots:
	void testStripDocObject();
private:
	bool _travis;

};

#include "PhTools/PhGenericSettings.h"
#include "PhGraphicStrip/PhGraphicStripSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class GraphicStripTestSettings : PhGenericSettings,
	public PhGraphicStripSettings,
	public PhDocumentWindowSettings

{
public:
	PH_SETTING_INT(setScreenDelay, screenDelay)

	// PhGraphicSettings
	PH_SETTING_BOOL(setDisplayInfo, displayInfo)
	PH_SETTING_BOOL(setResetInfo, resetInfo)

	// PhGraphicStripSettings :
	PH_SETTING_FLOAT2(setStripHeight, stripHeight, 0.25f)
	PH_SETTING_INT2(setHorizontalTimePerPixel, horizontalTimePerPixel, 80)
	PH_SETTING_INT2(setVerticalTimePerPixel, verticalTimePerPixel, 1000)
	PH_SETTING_STRING(setTextFontFile, textFontFile)
	PH_SETTING_INT2(setTextBoldness, textBoldness, 1)
	PH_SETTING_BOOL(setStripTestMode, stripTestMode)
	PH_SETTING_BOOL2(setDisplayNextText, displayNextText, true)
	PH_SETTING_BOOL(setInvertColor, invertColor)
	PH_SETTING_BOOL(setDisplayRuler, displayRuler)
	PH_SETTING_INT(setRulerTimeIn, rulerTimeIn)
	PH_SETTING_INT2(setTimeBetweenRuler, timeBetweenRuler, 48000)

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
	PH_SETTING_BOOL(setGenerate, generate)
	PH_SETTING_INT2(setPeopleNumber, peopleNumber, 1)
	PH_SETTING_INT2(setLoopNumber, loopNumber, 1)
	PH_SETTING_INT2(setTextNumber, textNumber, 10)
	PH_SETTING_INT2(setSpaceBetweenText, spaceBetweenText, 24000)
	PH_SETTING_INT2(setTrackNumber, trackNumber, 4)
	PH_SETTING_STRING2(setTextContent, textContent, "Per hoc minui studium suum existimans Paulus.")
	PH_SETTING_INT2(setStartTime, startTime, 0)
};

#endif // GRAPHICSTRIPTEST_H
