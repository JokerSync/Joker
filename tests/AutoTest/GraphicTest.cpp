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

	bool initCalled = false;
	bool paintCalled = false;

	view.registerInitialization([&]() {
	                                initCalled = true;
								});

	view.registerPaint([&](int w, int h) {
	                       paintCalled = true;
	                       QCOMPARE(w, 64);
	                       QCOMPARE(h, 64);
					   });

	view.show();

	QVERIFY(initCalled);
	QVERIFY(paintCalled);
}

void GraphicTest::rectTest()
{
	PhGraphicView view(32, 32);

	PhGraphicSolidRect rect(0, 0, 16, 16);
	rect.setColor(Qt::red);

	view.registerPaint([&](int w, int h) {
	                       rect.draw();
					   });

	view.show();

	QImage resultImage(view.grabFrameBuffer());
	QString resultFile = QString("%1.result.bmp").arg(QTest::currentTestFunction());
	resultImage.save(resultFile);
	QString expectedFile = QString("%1.expected.bmp").arg(QTest::currentTestFunction());
	QImage expectedImage(expectedFile);

	unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
	PHDEBUG << "result:" << result;
	QVERIFY(result == 0);
}

void GraphicTest::imageTest()
{
	int w = 64;
	int h = 64;
	PhGraphicView view(w, h);

	PhGraphicImage image("rgbPatternTest.expected.bmp", 0, 0, w, h);

	view.registerInitialization([&]() {
	                                QVERIFY(image.init());
								});

	view.registerPaint([&](int w, int h) {
	                       image.draw();
					   });

	view.show();

	QImage resultImage(view.grabFrameBuffer());
	QString resultFile = QString("%1.result.bmp").arg(QTest::currentTestFunction());
	resultImage.save(resultFile);
	// The expected result should be the same than the input (rgbPatternTest.bmp)
	// but it turns out that image is altered when opened with IMG_Load()
	// (see PhGraphicImage::init())
	QString expectedFile = QString("%1.expected.bmp").arg(QTest::currentTestFunction());
	QImage expectedImage(expectedFile);

	unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
	PHDEBUG << "result:" << result;
	QVERIFY(result == 0);
}

void GraphicTest::rgbPatternTest()
{
	int w = 64;
	int h = 64;
	PhGraphicView view(w, h);

	PhGraphicTexturedRect rect(0, 0, w, h);

	view.registerInitialization([&]() {
	                                unsigned char * buffer = PhPictureTools::generateRGBPattern(w, h);

	                                for(int i = 0; i < w; i++) {
	                                    for (int j = 0; j < h; j++) {

										}
									}
	                                rect.createTextureFromRGBBuffer(buffer, w, h);
	                                delete buffer;
								});

	view.registerPaint([&](int w, int h) {
	                       rect.draw();
					   });

	view.show();

	QImage resultImage(view.grabFrameBuffer());
	QString resultFile = QString("%1.result.bmp").arg(QTest::currentTestFunction());
	resultImage.save(resultFile);
	QString expectedFile = QString("%1.expected.bmp").arg(QTest::currentTestFunction());
	QImage expectedImage(expectedFile);

	unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
	PHDEBUG << "result:" << result;
	QVERIFY(result == 0);
}
