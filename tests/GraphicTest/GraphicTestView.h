/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef GraphicTestView_H
#define GraphicTestView_H

#include <QString>
#include <QMediaPlayer>

#include "PhGraphicView.h"
#include "PhGraphicImage.h"
#include "PhGraphicText.h"
#include "PhFont.h"
#include "PhGraphicSolidRect.h"
#include "QtDesigner.h"

#define TEXT
//#define IMAGE
//#define RECT

class GraphicTestView : public PhGraphicView
{
public :
	explicit GraphicTestView(QWidget *parent = 0, QString name = "");

	GraphicTestView *view();

public slots:
	void play();

protected:
	bool init();

	void paint();

private:

	bool playEnable;

	PhGraphicImage *_image;
	PhGraphicText *_text;
	PhFont *_font;
	PhGraphicSolidRect *_rect;

};

#endif // GraphicTestView_H
