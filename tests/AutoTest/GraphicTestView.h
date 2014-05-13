/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef GRAPHICTESTVIEW_H
#define GRAPHICTESTVIEW_H

#include <PhGraphic/PhGraphicView.h>

class GraphicTestView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit GraphicTestView(QWidget *parent = 0);

	QImage saveBuffer();

signals:
	void initSignal();
	void draw(int width, int height);

protected:
	bool init();

	void paint();
};

#endif // GRAPHICTESTVIEW_H
