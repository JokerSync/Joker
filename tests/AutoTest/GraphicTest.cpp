/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>

#include "PhGraphic/PhGraphicSolidRect.h"

#include "GraphicTest.h"
#include "GraphicTestView.h"

GraphicTest::GraphicTest(QObject *parent) :
	QObject(parent)
{
}

void GraphicTest::testPaint()
{
	GraphicTestView v;

	auto draw = [](int w, int h) {
					PHDEBUG << w << h;
					PhGraphicSolidRect rect(0, 0, w / 2, h / 2);

					rect.setColor(Qt::red);
					rect.draw();
				};

	connect(&v, &GraphicTestView::draw, draw);

	v.show();

	QImage grab = v.saveBuffer();
	grab.save("testPaintResult.bmp");
	QVERIFY(QImage("testPaint.bmp") == grab);
}
