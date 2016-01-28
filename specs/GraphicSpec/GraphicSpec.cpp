/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhGraphicTexturedRect.h"
#include "PhGraphic/PhGraphicImage.h"
#include "PhGraphic/PhGraphicLoop.h"

#include "PhSpec.h"
#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("graphic_test", [&](){
		before_each([&](){
			PhDebug::disable();
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

		it("draw_a_rect", [&](){
			PhGraphicView view(32, 32);

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				PhGraphicSolidRect rect(0, 0, w / 2, h / 2);
				rect.setColor(Qt::red);
				rect.draw();
			});

			AssertThat(compareImage(view.renderPixmap(32, 32).toImage(), QImage("rectTest.expected.bmp"), "rectTest"), IsTrue());
		});

		it("draw_an_image", [&](){
			PhGraphicView view(64, 64);

			PhGraphicImage image("rgbPatternTest.expected.bmp");

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				image.setSize(w, h);
				image.draw();
			});

			AssertThat(compareImage(view.renderPixmap(64, 64).toImage(), QImage("rgbPatternTest.expected.bmp"), "draw_an_image"), IsTrue());
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

			AssertThat(compareImage(view.renderPixmap(64, 64).toImage(), QImage("rgbPatternTest.expected.bmp"), "draw_a_rgb_pattern"), IsTrue());
		});

		it("draw a loop", [&]() {
			PhGraphicView view(64, 64);

			PhGraphicLoop loop(32, 0, 32, 64);

			loop.setCrossSize(16);
			loop.setThickness(4);
			loop.setHorizontalLoop(false);

			AssertThat(loop.crossSize(), Equals(16));
			AssertThat(loop.thickness(), Equals(4));

			QObject::connect(&view, &PhGraphicView::paint, [&](int, int) {
				loop.draw();
			});

			AssertThat(compareImage(view.renderPixmap(64, 64).toImage(), QImage("loopTest.expected.bmp"), "loopTest"), IsTrue());
		});

		it("draw an horizontal loop", [&]() {
			PhGraphicView view(64, 64);

			PhGraphicLoop loop(0, 32, 64, 32);

			loop.setCrossSize(16);
			loop.setThickness(4);
			loop.setHorizontalLoop(true);

			AssertThat(loop.crossSize(), Equals(16));
			AssertThat(loop.thickness(), Equals(4));

			QObject::connect(&view, &PhGraphicView::paint, [&](int, int) {
				loop.draw();
			});

			AssertThat(compareImage(view.renderPixmap(64, 64).toImage(), QImage("loopHorizontalTest.expected.bmp"), "loopHorizontalTest"), IsTrue());
		});
	});
});
