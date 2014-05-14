/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>

#include "PhGraphic/PhGraphicSolidRect.h"

#include "PhGraphic/PhGraphicView.h"

#include "GraphicTest.h"

GraphicTest::GraphicTest(QObject *parent) :
	QObject(parent)
{
}

void GraphicTest::initTestCase()
{
	PhDebug::enable();
}

void GraphicTest::testPaint()
{
	PhGraphicView v;
	v.setGeometry(0, 0, 16, 16);

	PhGraphicSolidRect rect(0, 0, 8, 8);
	rect.setColor(Qt::red);

	connect(&v, &PhGraphicView::paint, [&](int w, int h) {
	            rect.draw();
			});

	v.show();

	QImage grab = v.grabFrameBuffer();
	grab.save("testPaintResult.bmp");
	QVERIFY(QImage("testPaint.bmp") == grab);
}
