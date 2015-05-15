/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhGeneric.h"
#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhGraphicTexturedRect.h"
#include "PhGraphic/PhGraphicImage.h"

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

#warning /// @todo remove if unnecessary
			//		QApplication::processEvents();

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

#warning /// @todo remove if unnecessary
			//		QApplication::processEvents();

			// Note: using view.grabFrameBuffer() causes timing issues.
			// There is no warranty that the scene has been fully rendered.
			// Use view.renderPixmap().toImage() instead.
			QImage resultImage(view.renderPixmap(32, 32).toImage());
			QString resultFile = "rectTest.result.bmp";
			resultImage.save(resultFile);
			QString expectedFile = "rectTest.expected.bmp";
			QImage expectedImage(expectedFile);

			unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
			AssertThat(result, Equals(0));
		});

		it("draw_an_image", [&](){
			PhGraphicView view(64, 64);

			PhGraphicImage image("rgbPatternTest.expected.bmp");

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
				image.setSize(w, h);
				image.draw();
			});

			view.show();

			QImage resultImage(view.renderPixmap(64, 64).toImage());
			QString resultFile = "imageTest.result.bmp";
			resultImage.save(resultFile);

			// The expected result should be the same than the input (rgbPatternTest.expected.bmp)
			// but it turns out that image is altered when opened with IMG_Load()
			// (see PhGraphicImage::init())
			QString expectedFile = "imageTest.expected.bmp";
			QImage expectedImage(expectedFile);

			unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
			AssertThat(result, Equals(0));
		});

		it("draw_a_rgb_pattern", [&](){
			int w = 64;
			int h = 64;
			PhGraphicView view(w, h);

			PhGraphicTexturedRect rect(0, 0, w, h);

			QObject::connect(&view, &PhGraphicView::paint, [&](int w, int h) {
#warning /// @todo try to make it before (creating the texture on auto init)
				unsigned char * buffer = PhPictureTools::generateRGBPattern(w, h);
				rect.createTextureFromRGBBuffer(buffer, w, h);
				delete buffer;
				rect.draw();
			});

			view.show();

			QImage resultImage(view.renderPixmap(64, 64).toImage());
			QString resultFile = "rgbPatternTest.result.bmp";
			resultImage.save(resultFile);
			QString expectedFile = "rgbPatternTest.expected.bmp";
			QImage expectedImage(expectedFile);

			unsigned int result = PhPictureTools::compare(resultImage, expectedImage);
			AssertThat(result, Equals(0));
		});
	});
});
