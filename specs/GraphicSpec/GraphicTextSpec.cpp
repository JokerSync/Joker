/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicSolidRect.h"

#include "PhSpec.h"
#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("graphic_text_test", [&](){
		PhGraphicView *view;
		PhFont *font;

		before_each([&](){
			PhDebug::disable();

			view = new PhGraphicView(776, 576);
			font = new PhFont();

			QObject::connect(view, &PhGraphicView::paint, [&](int, int) {
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				int textHeight = 50;
				int textWidth = 600;

				PhGraphicSolidRect rect(0, 0, 900, textHeight);
				rect.setColor(Qt::yellow);
				rect.setZ(2);

				QString lowerCase("abcdefghijklmnopqrstuvwxyz");
				PhGraphicText text1(font, lowerCase);
				text1.setRect(0, 0, textWidth, textHeight);
				text1.setColor(Qt::black);
				text1.setZ(5);
				text1.draw();

				rect.setY(100);
				rect.draw();
				QString upperCase(lowerCase.toUpper());
				PhGraphicText text2(font, upperCase);
				text2.setRect(0, 100, textWidth, textHeight);
				text2.setColor(Qt::black);
				text2.setZ(5);
				text2.draw();

				rect.setY(200);
				rect.draw();
				QString specials("à â ç è é ê \nî ô ù û");
				PhGraphicText text3(font, specials);
				text3.setRect(0, 200, textWidth, textHeight);
				text3.setColor(Qt::black);
				text3.setZ(5);
				text3.draw();

				rect.setY(300);
				rect.draw();
				QString numbers("0 1 2 3 4 5 6 7 8 9 0");
				PhGraphicText text4(font, numbers);
				text4.setRect(0, 300, textWidth, textHeight);
				text4.setColor(Qt::black);
				text4.setZ(5);
				text4.draw();
			});
		});

		after_each([&](){
			delete font;
			delete view;
		});

		it("draw_swenson_font", [&](){
			font->setFontFile("SWENSON.ttf");
			AssertThat(font->ready(), IsFalse());

			AssertThat(compareImage(
						   view->renderPixmap(776, 576).toImage(),
						   QImage("fontTest.SWENSON.ttf.expected.bmp"),
						   "fontTest.SWENSON.ttf",
						   776 * 576 * 0.04),
					   IsTrue());

			AssertThat(font->ready(), IsTrue());
		});

		it("draw_arial_font", [&](){
			font->setFontFile("Arial.ttf");

			AssertThat(compareImage(
						   view->renderPixmap(776, 576).toImage(),
						   QImage("fontTest.Arial.ttf.expected.bmp"),
						   "fontTest.Arial.ttf",
						   776 * 576 * 0.04),
					   IsTrue());
		});

		it("draw_bedizen_font", [&](){
			font->setFontFile("Bedizen.ttf");

			AssertThat(compareImage(
						   view->renderPixmap(776, 576).toImage(),
						   QImage("fontTest.Bedizen.ttf.expected.bmp"),
						   "fontTest.Bedizen.ttf",
						   776 * 576 * 0.04),
					   IsTrue());
		});

		it("draw_bad_font", [&](){
			font->setFontFile("bad_font.ttf");

			AssertThat(compareImage(
						   view->renderPixmap(776, 576).toImage(),
						   QImage("fontTest.bad_font.ttf.expected.bmp"),
						   "fontTest.bad_font.ttf",
						   776 * 576 * 0.04),
					   IsTrue());
		});
	});
});
