/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>

#include "PhTools/PhPictureTools.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhGraphicTexturedRect.h"
#include "PhGraphic/PhGraphicImage.h"


#include "GraphicTest.h"

#define WAIT_TIME 40

GraphicTest::GraphicTest(QObject *parent) :
	QObject(parent)
{
}

void GraphicTest::initTestCase()
{
	PhDebug::enable();
}

void GraphicTest::paintTest()
{
	PhGraphicView view(64, 64);

	bool paintCalled = false;

	connect(&view, &PhGraphicView::paint, [&](int w, int h) {
	            paintCalled = true;
	            QCOMPARE(w, 64);
	            QCOMPARE(h, 64);
			});

	view.show();
	QTest::qWait(WAIT_TIME);

	QVERIFY(paintCalled);
}

void GraphicTest::rectTest()
{
	PhGraphicView view(32, 32);

	PhGraphicSolidRect rect;
	rect.setColor(Qt::red);

	connect(&view, &PhGraphicView::paint, [&](int w, int h) {
	            rect.setSize(w / 2, h / 2);
	            rect.draw();
			});

	view.show();

	// Note: using view.grabFrameBuffer() causes timing issues.
	// There is no warranty that the scene has been fully rendered.
	// Use view.renderPixmap().toImage() instead.
	QImage resultImage(view.renderPixmap().toImage());
	QString resultFile = QString("%1.result.bmp").arg(QTest::currentTestFunction());
	resultImage.save(resultFile);
	QString expectedFile = QString("%1.expected.bmp").arg(QTest::currentTestFunction());
	QImage expectedImage(expectedFile);

	unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
	QVERIFY2(result == 0, PHNQ(QString("Comparison result=%1").arg(result)));
}

void GraphicTest::imageTest()
{
	PhGraphicView view(64, 64);

	PhGraphicImage image("rgbPatternTest.expected.bmp");

	connect(&view, &PhGraphicView::paint, [&](int w, int h) {
	            image.setSize(w, h);
	            image.draw();
			});

	view.show();

	QImage resultImage(view.renderPixmap().toImage());
	QString resultFile = QString("%1.result.bmp").arg(QTest::currentTestFunction());
	resultImage.save(resultFile);

	// The expected result should be the same than the input (rgbPatternTest.bmp)
	// but it turns out that image is altered when opened with IMG_Load()
	// (see PhGraphicImage::init())
	QString expectedFile = QString("%1.expected.bmp").arg(QTest::currentTestFunction());
	QImage expectedImage(expectedFile);

	unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
	QVERIFY2(result == 0, PHNQ(QString("Comparison result=%1").arg(result)));
}

void GraphicTest::rgbPatternTest()
{
	int w = 64;
	int h = 64;
	PhGraphicView view(w, h);

	PhGraphicTexturedRect rect(0, 0, w, h);


	connect(&view, &PhGraphicView::paint, [&](int w, int h) {
#warning /// @todo try to make it before (creating the texture on auto init)
	            unsigned char * buffer = PhPictureTools::generateRGBPattern(w, h);
	            rect.createTextureFromRGBBuffer(buffer, w, h);
	            delete buffer;
	            rect.draw();
			});

	view.show();

	QImage resultImage(view.renderPixmap().toImage());
	QString resultFile = QString("%1.result.bmp").arg(QTest::currentTestFunction());
	resultImage.save(resultFile);
	QString expectedFile = QString("%1.expected.bmp").arg(QTest::currentTestFunction());
	QImage expectedImage(expectedFile);

	unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
	QVERIFY2(result == 0, PHNQ(QString("Comparison result=%1").arg(result)));
}
