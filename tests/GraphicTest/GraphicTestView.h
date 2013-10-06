/**
* Copyright (C) 2012-2013 Phonations
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

class GraphicTestView : public PhGraphicView
{
	Q_OBJECT
public :
	explicit GraphicTestView(QWidget *parent = 0);
protected:
	bool init();

	void paint();

private:
	PhGraphicImage *_image;
	PhFont *_font;
	PhGraphicText *_text;
	PhGraphicSolidRect *_rect;
	PhGraphicLoop * _loop;
};

#endif // GraphicTestView_H
