/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <exception>

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhGraphicTexturedRect.h"
#include "PhGraphic/PhGraphicImage.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("graphic_test", [&](){
		before_each([&](){
			PhDebug::setLogMask((1 << 9) | PHDEBUG_SPEC_MASK);
		});

		it("call_paint_when_shown", [&](){
			PhGraphicView view(64, 64);

			bool paintCalled = false;

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				paintCalled = true;
				AssertThat(w, Equals(64));
				AssertThat(h, Equals(64));
			});

			view.renderPixmap(64, 64);

			AssertThat(paintCalled, IsTrue());
		});

		it("compare", [&](){
			PhGraphicView view(32, 32);

			QFile unexistingResultFile("compareTest.unexisting.result.bmp");
			if(unexistingResultFile.exists()) {
				unexistingResultFile.remove();
			}
			AssertThat(unexistingResultFile.exists(), IsFalse());
            AssertThat(view.compare("compareTest.unexisting.bmp"), Equals(-10));
			AssertThat(unexistingResultFile.exists(), IsTrue());
			unexistingResultFile.remove();

			QFile badSizeResultFile("compareTest.64x32.result.bmp");
			if(badSizeResultFile.exists()) {
				badSizeResultFile.remove();
			}
			AssertThat(badSizeResultFile.exists(), IsFalse());
            AssertThat(view.compare("compareTest.64x32.bmp"), Equals(-20));
			AssertThat(badSizeResultFile.exists(), IsTrue());
			badSizeResultFile.remove();

			AssertThat(view.compare("compareTest.64x32.bmp", 0, 64, 32), Equals(0));

			QFile smallDiffResultFile("compareTest.smalldiff.result.bmp");
			if(smallDiffResultFile.exists()) {
				smallDiffResultFile.remove();
			}
			AssertThat(smallDiffResultFile.exists(), IsFalse());
			AssertThat(view.compare("compareTest.smalldiff.bmp"), Equals(14));
			AssertThat(smallDiffResultFile.exists(), IsTrue());
			smallDiffResultFile.remove();

			AssertThat(view.compare("compareTest.smalldiff.bmp", 14), Equals(14));
			AssertThat(smallDiffResultFile.exists(), IsFalse());

			AssertThat(view.compare("compareTest.nodiff.bmp"), Equals(0));
		});

		it("compare big difference", [&](){
			PhGraphicView view(128, 128);

            // an image where the pixel difference is more than the maximum of a 32 bits integer
			QFile file("compareTest.bigdiff.result.bmp");
			if(file.exists()) {
				file.remove();
			}
			AssertThat(file.exists(), IsFalse());
            AssertThat(view.compare("compareTest.bigdiff.bmp"), Equals(3196108800));

			AssertThat(file.exists(), IsTrue());
			file.remove();
		});

		it("draw_a_rect", [&](){
			PhGraphicView view(32, 32);

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				PhGraphicSolidRect rect(0, 0, w / 2, h / 2);
				rect.setColor(Qt::red);
				rect.draw();
			});

			AssertThat(view.compare("rectTest.bmp"), Equals(0));
		});

		it("draw_an_image", [&](){
			PhGraphicView view(64, 64);

			PhGraphicImage image("rgbPatternTest.bmp");

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				image.setSize(w, h);
				image.draw();
			});

			AssertThat(view.compare("rgbPatternTest.bmp"), Equals(0));
		});

		it("draw_a_rgb_pattern", [&](){
			int w = 64;
			int h = 64;
			PhGraphicView view(w, h);

			PhGraphicTexturedRect rect(0, 0, w, h);

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				unsigned char * buffer = PhPictureTools::generateRGBPattern(w, h);
				rect.createTextureFromRGBBuffer(buffer, w, h);
				delete buffer;
				rect.draw();
			});

			AssertThat(view.compare("rgbPatternTest.bmp"), Equals(0));
		});
	});
});
