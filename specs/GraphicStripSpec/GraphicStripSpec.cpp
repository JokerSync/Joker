/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphicStrip/PhGraphicStrip.h"

#include "GraphicStripSpecSettings.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("graphic_strip_test", []() {
		before_each([&](){
			PhDebug::setLogMask((1 << 9) | PHDEBUG_SPEC_MASK);
		});

		it("draw_a_graphic_strip", [&](){
			PhGraphicView view(720, 240);

			GraphicStripSpecSettings settings;
			PhGraphicStrip strip(&settings);

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				strip.draw(0, 0, w, h);
			});

			PhStripDoc * doc = strip.doc();
			doc->reset();
			doc->addPeople(new PhPeople("A people"));
			doc->addPeople(new PhPeople("A second people", "red"));

			doc->addText(new PhStripText(0, doc->peoples().first(), 10000, 0.25f, "Hello", 0.25f));
			doc->addCut(new PhStripCut(5400, PhStripCut::CrossFade));
			doc->addDetect(new PhStripDetect(PhStripDetect::Off, 0, doc->peoples().first(), 10000, 0.25f));
			doc->addLoop(new PhStripLoop(22000, "label"));
			doc->addText(new PhStripText(10000, doc->peoples().last(), 15000, 0.5f, "Hi !", 0.25f));
			doc->addDetect(new PhStripDetect(PhStripDetect::SemiOff, 10000, doc->peoples().last(), 15000, 0.5f));

			AssertThat(view.compare("drawTest.expected.bmp"), IsLessThan(720 * 240 * 4));
		});
	});
});

