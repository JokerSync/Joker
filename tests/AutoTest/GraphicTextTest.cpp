/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include <QTest>
#include <QThread>

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicSolidRect.h"

#include "PhTools/PhPictureTools.h"

#include "GraphicTextTest.h"

void GraphicTextTest::fontTest() {

	QFETCH(QString, fontFile);

	QString imageFileNameTemplate = QString("%1.%2.%3.bmp")
	                                .arg(QTest::currentTestFunction())
	                                .arg(fontFile);

	PhGraphicView view(776, 576);

	PhFont font;
	font.setFontFile(fontFile);

	connect(&view, &PhGraphicView::paint, [&](int w, int h) {
	            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	            int textHeight = 50;
	            int textWidth = 600;

	            PhGraphicSolidRect rect(0, 0, 900, textHeight);
	            rect.setColor(Qt::yellow);
	            rect.setZ(2);

	            QString lowerCase("abcdefghijklmnopqrstuvwxyz");
	            PhGraphicText text1(&font, lowerCase);
	            text1.setRect(0, 0, textWidth, textHeight);
	            text1.setColor(Qt::black);
	            text1.setZ(5);
	            text1.draw();

	            rect.setY(100);
	            rect.draw();
	            QString upperCase(lowerCase.toUpper());
	            PhGraphicText text2(&font, upperCase);
	            text2.setRect(0, 100, textWidth, textHeight);
	            text2.setColor(Qt::black);
	            text2.setZ(5);
	            text2.draw();

	            rect.setY(200);
	            rect.draw();
	            QString specials("à â ç è é ê \nî ô ù û");
	            PhGraphicText text3(&font, specials);
	            text3.setRect(0, 200, textWidth, textHeight);
	            text3.setColor(Qt::black);
	            text3.setZ(5);
	            text3.draw();

	            rect.setY(300);
	            rect.draw();
	            QString numbers("0 1 2 3 4 5 6 7 8 9 0");
	            PhGraphicText text4(&font, numbers);
	            text4.setRect(0, 300, textWidth, textHeight);
	            text4.setColor(Qt::black);
	            text4.setZ(5);
	            text4.draw();
			});

	QString expectedFile = imageFileNameTemplate.arg("expected");

	view.show();

	QImage resultImage(view.renderPixmap().toImage());
	resultImage.save(imageFileNameTemplate.arg("result"));
	QImage expectedImage(expectedFile);

	int result = PhPictureTools::compare(resultImage, expectedImage);
	PHDEBUG << result;
	QVERIFY2(result < 776 * 576 / 50, PHNQ(expectedFile)); // accept a difference of 1 per 4 pixels
}

void GraphicTextTest::fontTest_data()
{
	QTest::addColumn<QString>("fontFile");

	QTest::newRow("SWENSON.TTF") << "SWENSON.TTF";
	QTest::newRow("Arial.ttf") << "Arial.ttf";
	QTest::newRow("weblyleekuil.ttf") << "weblysleekuil.ttf";
	QTest::newRow("Bedizen.ttf") << "Bedizen.ttf";
	QTest::newRow("bad font") << "Bad_font.ttf";
}

void GraphicTextTest::computeMaxFontSizeTest()
{
	PhGraphicView view;

	QCOMPARE(PhFont::computeMaxFontSize("Arial.ttf"), 113);
	QCOMPARE(PhFont::computeMaxFontSize("SWENSON.ttf"), 124);
	QCOMPARE(PhFont::computeMaxFontSize("Bedizen.ttf"), 97);
	QCOMPARE(PhFont::computeMaxFontSize("weblysleekuil.ttf"), 94);
}
