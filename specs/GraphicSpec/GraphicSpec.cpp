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

			view.show();

			AssertThat(paintCalled, IsTrue());
		});

		it("draw_a_rect", [&](){
			PhGraphicView view(32, 32);

			PhGraphicSolidRect rect;
			rect.setColor(Qt::red);

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				rect.setSize(w / 2, h / 2);
				rect.draw();
			});

			view.show();

			// Note: using view.grabFrameBuffer() causes timing issues.
			// There is no warranty that the scene has been fully rendered.
			// Use view.renderPixmap().toImage() instead.
			QImage resultImage(view.renderPixmap(32, 32).toImage());
			QString resultFile = "rectTest.result.bmp";
			resultImage.save(resultFile);
			QString expectedFile = "rectTest.expected.bmp";
			QImage expectedImage(expectedFile);

			unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
			AssertThat(result, Equals((unsigned int)0));
		});

		it("draw_an_image", [&](){
			PhGraphicView view(64, 64);

			PhGraphicImage image("rgbPatternTest.expected.bmp");

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				image.setSize(w, h);
				image.draw();
			});

			AssertThat(QImage(view.renderPixmap(64, 64).toImage()) == QImage("rgbPatternTest.expected.bmp"), IsTrue());
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

			AssertThat(QImage(view.renderPixmap(64, 64).toImage()) == QImage("rgbPatternTest.expected.bmp"), IsTrue());
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

			view.show();

			QImage resultImage(view.renderPixmap(64, 64).toImage());
			QString resultFile = "loopTest.result.bmp";
			resultImage.save(resultFile);
			QString expectedFile = "loopTest.expected.bmp";
			QImage expectedImage(expectedFile);

			unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
			AssertThat(result, Equals((unsigned int)0));
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

			view.show();

			QImage resultImage(view.renderPixmap(64, 64).toImage());
			QString resultFile = "loopHorizontalTest.result.bmp";
			resultImage.save(resultFile);
			QString expectedFile = "loopHorizontalTest.expected.bmp";
			QImage expectedImage(expectedFile);

			unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
			AssertThat(result, Equals((unsigned int)0));
		});
	});
});
