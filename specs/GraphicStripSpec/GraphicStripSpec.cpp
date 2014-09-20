/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QThread>

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphicStrip/PhGraphicStrip.h"

#include "GraphicStripSpecSettings.h"

using namespace igloo;

Describe(graphic_strip_test) {
	static void SetUpContext() {
		//PhDebug::disable();
	}

	It(draw_a_graphic_strip) {
		PhGraphicView view(980, 320);

		GraphicStripSpecSettings settings;
		PhGraphicStrip strip(&settings);

		QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
			strip.draw(0, 0, w, h);
		});

		PhStripDoc * doc = strip.doc();
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

		view.show();

		QImage resultImage(view.renderPixmap(980, 320).toImage());
		QString resultFile = "drawTest.result.bmp";
		resultImage.save(resultFile);
		QString expectedFile = "drawTest.expected.bmp";
		QImage expectedImage(expectedFile);

		unsigned int result = PhPictureTools::compare(resultImage, expectedImage, true);
		PHDEBUG << "result:" << result;
		AssertThat(result, IsLessThan(920 * 320 * 0.04)); // accept a difference of 4%
	}
};

