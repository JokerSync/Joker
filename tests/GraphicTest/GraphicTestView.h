/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef GraphicTestView_H
#define GraphicTestView_H

#include <QString>
#include <QMediaPlayer>

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicImage.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhFont.h"
#include "PhGraphic/PhGraphicSolidRect.h"

#define TEXT
#define IMAGE
#define RECT

class GraphicTestView : public PhGraphicView
{
public :
	explicit GraphicTestView(QWidget *parent = 0, QString name = "");

	GraphicTestView *view();

protected:
	bool init();

	void paint();

private:
#ifdef IMAGE
	PhGraphicImage *_image;
#endif

#ifdef TEXT
	PhGraphicText *_text;
	PhFont *_font;
#endif

#ifdef RECT
	PhGraphicSolidRect *_rect;
#endif
};

#endif // GraphicTestView_H
