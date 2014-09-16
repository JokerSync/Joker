/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QThread>

#include "igloo/igloo_alt.h"

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicSolidRect.h"

using namespace igloo;

Describe(graphic_text_test) {
	static void SetUpContext() {
		PhDebug::disable();
	}

	PhGraphicView *view;
	PhFont font;

	void SetUp() {
		view = new PhGraphicView(776, 576);
		QObject::connect(view, &PhGraphicView::paint, [&](int w, int h) {
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
	}

	void TearDown() {
		delete view;
	}

	It(draw_swenson_font) {
		font.setFontFile("SWENSON.ttf");

		QImage resultImage(view->renderPixmap(776, 576).toImage());
		resultImage.save("fontTest.SWENSON.ttf.result.bmp");
		QImage expectedImage("fontTest.SWENSON.ttf.expected.bmp");

		int result = PhPictureTools::compare(resultImage, expectedImage);
		Assert::That(result, IsLessThan(776 * 576 * 0.04)); // accept a difference of 4% pixels
	}

	It(draw_arial_font) {
		font.setFontFile("Arial.ttf");

		QImage resultImage(view->renderPixmap(776, 576).toImage());
		resultImage.save("fontTest.Arial.ttf.result.bmp");
		QImage expectedImage("fontTest.Arial.ttf.expected.bmp");

		int result = PhPictureTools::compare(resultImage, expectedImage);
		Assert::That(result, IsLessThan(776 * 576 * 0.04)); // accept a difference of 4% pixels
	}

	It(draw_bedizen_font) {
		font.setFontFile("Bedizen.ttf");

		QImage resultImage(view->renderPixmap(776, 576).toImage());
		resultImage.save("fontTest.Bedizen.ttf.result.bmp");
		QImage expectedImage("fontTest.Bedizen.ttf.expected.bmp");

		int result = PhPictureTools::compare(resultImage, expectedImage);
		Assert::That(result, IsLessThan(776 * 576 * 0.04)); // accept a difference of 4% pixels
	}

	It(draw_bad_font) {
		font.setFontFile("bad_font.ttf");

		QImage resultImage(view->renderPixmap(776, 576).toImage());
		resultImage.save("fontTest.bad_font.ttf.result.bmp");
		QImage expectedImage("fontTest.bad_font.ttf.expected.bmp");

		int result = PhPictureTools::compare(resultImage, expectedImage);
		Assert::That(result, IsLessThan(776 * 576 * 0.04)); // accept a difference of 4% pixels
	}
};

