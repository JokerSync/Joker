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

class GraphicTestView : public PhGraphicView
{
public :
	explicit GraphicTestView(QWidget *parent = 0);

	GraphicTestView *view();

public slots:
	void play();
	void fastBackward();
	void fastForward();

protected:
	bool init();

	void paint();

private:

	int playEnable;
	int textSpeed;

	PhGraphicImage *_image;
	PhGraphicText *_text;
	PhFont *_font;
	PhGraphicSolidRect *_rect;

};

#endif // GraphicTestView_H
