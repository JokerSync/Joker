/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>
#include <QWindow>

#include "PhTools/PhPictureTools.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphicStrip/PhGraphicStrip.h"

#include "GraphicStripTestSettings.h"

#include "GraphicStripTest.h"

void GraphicStripTest::drawTest()
{
	PhGraphicView view(980, 320);

	PhGraphicStrip _strip;
	GraphicStripTestSettings _settings;
	_strip.setSettings(&_settings);

	view.registerInitialization([&]() {
	                                _strip.init();
								});

	view.registerPaint([&](int width, int height) {
	                       PHDEBUG << width << height;
	                       _strip.draw(0, 0, width, height);
					   });

	PhStripDoc * doc = _strip.doc();
	doc->reset();
	doc->addPeople(new PhPeople("A people"));
	doc->addPeople(new PhPeople("A second people", "red"));

	doc->addObject(new PhStripText(0, doc->peoples().first(), 10000, 1, "Hello"));
	doc->addObject(new PhStripCut(PhStripCut::CrossFade, 5400));
	doc->addObject(new PhStripDetect(PhStripDetect::Off, 0, doc->peoples().first(), 10000, 1));
	doc->addObject(new PhStripLoop(3, 22000));
	doc->addObject(new PhStripText(10000, doc->peoples().last(), 15000, 2, "Hi !"));
	doc->addObject(new PhStripDetect(PhStripDetect::SemiOff, 10000, doc->peoples().last(), 15000, 2));
	doc->changed();

	view.show();

	QImage resultImage(view.grabFrameBuffer());
	QString resultFile = QString("%1.result.bmp").arg(QTest::currentTestFunction());
	resultImage.save(resultFile);
	QString expectedFile = QString("%1.expected.bmp").arg(QTest::currentTestFunction());
	QImage expectedImage(expectedFile);

	unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
	PHDEBUG << "result:" << result;
	QVERIFY(result < 920 * 320 / 4);
}
