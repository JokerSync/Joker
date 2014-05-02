/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include <QTest>
#include <QThread>

#include "GraphicTextTest.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicSolidRect.h"

GraphicTextTest::GraphicTextTest(QWidget *parent)
	: PhGraphicView( NULL)
{
}

void GraphicTextTest::testSwenson() {
	_font.setFontFile(QCoreApplication::applicationDirPath() + "/SWENSON.TTF");
	setGeometry(0, 0, 776, 576);

	QString expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsSWENSON.bmp");
	if(windowHandle()->devicePixelRatio() == 2)
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsSWENSON2.bmp");
	if(QString(qgetenv("TRAVIS")) == "true")
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsSWENSONTravis.bmp");

	show();
	paint();
	sleep(2);
	QImage img(grabFrameBuffer());
	img.save("./resultSwenson.bmp");
	QVERIFY(QImage(grabFrameBuffer()) == QImage(expectedFile));
}

void GraphicTextTest::testArial() {
	_font.setFontFile(QCoreApplication::applicationDirPath() + "/Arial.ttf");
	setGeometry(0, 0, 776, 576);

	QString expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsArial.bmp");
	if(windowHandle()->devicePixelRatio() == 2)
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsArial2.bmp");
	if(QString(qgetenv("TRAVIS")) == "true")
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsArialTravis.bmp");

	show();
	paint();
	sleep(2);
	QImage img(grabFrameBuffer());
	img.save("./resultArial.bmp");
	QVERIFY(QImage(grabFrameBuffer()) == QImage(expectedFile));

}

void GraphicTextTest::testWeblysleekuil()
{
	_font.setFontFile(QCoreApplication::applicationDirPath() + "/weblysleekuil.ttf");
	setGeometry(0, 0, 776, 576);

	QString expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsWeblysleekuil.bmp");
	if(windowHandle()->devicePixelRatio() == 2)
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsWeblysleekuil2.bmp");
	if(QString(qgetenv("TRAVIS")) == "true")
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsWeblysleekuilTravis.bmp");

	show();
	paint();
	sleep(2);
	QImage img(grabFrameBuffer());
	img.save("./resultWeblysleekuil.bmp");
	QVERIFY(QImage(grabFrameBuffer()) == QImage(expectedFile));
}

void GraphicTextTest::testBedizen() {
	_font.setFontFile(QCoreApplication::applicationDirPath() + "/Bedizen.ttf");
	setGeometry(0, 0, 776, 576);

	QString expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsBedizen.bmp");
	if(windowHandle()->devicePixelRatio() == 2)
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsBedizen2.bmp");
	if(QString(qgetenv("TRAVIS")) == "true")
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/charsBedizenTravis.bmp");

	show();
	paint();
	sleep(2);
	QImage img(grabFrameBuffer());
	img.save("./resultBedizen.bmp");
	QVERIFY(QImage(grabFrameBuffer()) == QImage(expectedFile));

}

void GraphicTextTest::computeMaxFontSizeTest()
{
	QCOMPARE(PhFont::computeMaxFontSize(QCoreApplication::applicationDirPath() + "/Arial.ttf"), 113);
	QCOMPARE(PhFont::computeMaxFontSize(QCoreApplication::applicationDirPath() + "/SWENSON.ttf"), 124);
	QCOMPARE(PhFont::computeMaxFontSize(QCoreApplication::applicationDirPath() + "/Bedizen.ttf"), 97);
	QCOMPARE(PhFont::computeMaxFontSize(QCoreApplication::applicationDirPath() + "/weblysleekuil.ttf"), 94);
}

void GraphicTextTest::paint() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


	int textHeight = 50;
	int textWidth = 600;

	PhGraphicSolidRect rect(0, 0, 900, textHeight);
	rect.setColor(Qt::yellow);
	rect.setZ(2);

	QString lowerCase("abcdefghijklmnopqrstuvwxyz");
	PhGraphicText text1(&_font, lowerCase);
	text1.setRect(0, 0, textWidth, textHeight);
	text1.setColor(Qt::black);
	text1.setZ(5);
	text1.draw();

	rect.setY(100);
	rect.draw();
	QString upperCase(lowerCase.toUpper());
	PhGraphicText text2(&_font, upperCase);
	text2.setRect(0, 100, textWidth, textHeight);
	text2.setColor(Qt::black);
	text2.setZ(5);
	text2.draw();

	rect.setY(200);
	rect.draw();
	QString specials("à â ç è é ê î ô ù û");
	PhGraphicText text3(&_font, specials);
	text3.setRect(0, 200, textWidth, textHeight);
	text3.setColor(Qt::black);
	text3.setZ(5);
	text3.draw();

	rect.setY(300);
	rect.draw();
	QString numbers("0 1 2 3 4 5 6 7 8 9 0");
	PhGraphicText text4(&_font, numbers);
	text4.setRect(0, 300, textWidth, textHeight);
	text4.setColor(Qt::black);
	text4.setZ(5);
	text4.draw();
}
