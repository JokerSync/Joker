/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>
#include <QWindow>

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphicStrip/PhGraphicStrip.h"

#include "GraphicStripTestSettings.h"

#include "GraphicStripTest.h"

void GraphicStripTest::testStripDocObject()
{
	PhGraphicView view;
	view.setGeometry(0, 0, 981, 319);

	PhGraphicStrip _strip;
	GraphicStripTestSettings _settings;
	_strip.setSettings(&_settings);
	connect(&view, &PhGraphicView::paint, [&](int width, int height) {
	            _strip.init();
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

	QTest::qWait(1000);

	QImage impr(view.grabFrameBuffer());
	impr.save("graphicStripTestResult.bmp");
	QString expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/graphicStripTest.bmp");
	if(view.windowHandle()->devicePixelRatio() == 2)
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/graphicStripRetinaTest.bmp");
	if(QString(qgetenv("TRAVIS")) == "true")
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/graphicStripTravisTest.bmp");

	QVERIFY2(impr == QImage(expectedFile), PHNQ(expectedFile));
}
