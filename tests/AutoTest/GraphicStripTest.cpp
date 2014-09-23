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
	PhGraphicView view(720, 240);

	GraphicStripTestSettings _settings;
	PhGraphicStrip _strip(&_settings);

	connect(&view, &PhGraphicView::paint, [&](int w, int h) {
	            _strip.draw(0, 0, w, h);
			});

	PhStripDoc * doc = _strip.doc();
	doc->reset();
	doc->addPeople(new PhPeople("A people"));
	doc->addPeople(new PhPeople("A second people", "red"));

	doc->addObject(new PhStripText(0, doc->peoples().first(), 10000, 0.25f, "Hello", 0.25f));
	doc->addObject(new PhStripCut(5400, PhStripCut::CrossFade));
	doc->addObject(new PhStripDetect(PhStripDetect::Off, 0, doc->peoples().first(), 10000, 0.25f));
	doc->addObject(new PhStripLoop(22000, "label"));
	doc->addObject(new PhStripText(10000, doc->peoples().last(), 15000, 0.5f, "Hi !", 0.25f));
	doc->addObject(new PhStripDetect(PhStripDetect::SemiOff, 10000, doc->peoples().last(), 15000, 0.5f));
	doc->changed();

	QImage resultImage(view.renderPixmap(720, 240).toImage());
	QString resultFile = QString("%1.result.bmp").arg(QTest::currentTestFunction());
	resultImage.save(resultFile);
	QString expectedFile = QString("%1.expected.bmp").arg(QTest::currentTestFunction());
	QImage expectedImage(expectedFile);

	unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
	PHDEBUG << "result:" << result;
	QVERIFY(result < 720 * 240); // accept a difference of 1 per pixel
}
