/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef GRAPHICTEXTTEST_H
#define GRAPHICTEXTTEST_H

#include <QObject>

#include "PhGraphic/PhGraphicView.h"


class GraphicTextTest : public PhGraphicView
{
	Q_OBJECT
public:
	explicit GraphicTextTest(QWidget *parent = 0);

signals:

public slots:

private slots:
	void testBedizen();

	void testSwenson();
	void testArial();
private:
	PhFont _font;
	void paint();

};

#endif // GRAPHICTEXTTEST_H
