/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef GraphicTestView_H
#define GraphicTestView_H

#include <QString>

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicImage.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhFont.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhGraphicLoop.h"
#include "PhGraphic/PhGraphicDisc.h"

#include "GraphicTestSettings.h"

class GraphicTestView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit GraphicTestView(QWidget *parent = 0);
	bool setFontFile(QString fontFile);

	void setTestSettings(GraphicTestSettings *settings);
protected:
	bool init();

	void paint();

private:
	GraphicTestSettings *_settings;
	PhGraphicImage _image;
	PhFont _font1, _font2;
	PhGraphicSolidRect _rect;
	PhGraphicTexturedRect _yuvRect;
	int _x;
};

#endif // GraphicTestView_H
