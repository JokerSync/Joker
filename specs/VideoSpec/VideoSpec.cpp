/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"
#include "PhGraphic/PhGraphicView.h"

#include "PhVideo/PhVideoEngine.h"

#include "VideoSpecSettings.h"

#define FRAME_WAIT_TIME 80

#include "PhSpec.h"
#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("video", [](){
		PhGraphicView *view;
		VideoSpecSettings *settings;
		PhVideoEngine *engine;
		int factor;

		before_each([&](){
			PhDebug::setLogMask(PHDEBUG_SPEC_MASK);

			view = new PhGraphicView(64, 64);
			settings = new VideoSpecSettings();
			engine = new PhVideoEngine(settings);

			view->show();

			engine->setBilinearFiltering(false);

			factor = 1;

			QObject::connect(view, &PhGraphicView::paint, [&](int w, int h) {
				engine->drawVideo(0, 0, factor * w, factor * h);
			});
		});

		after_each([&](){
			engine->close();

			delete engine;
			delete settings;
			delete view;
		});

		it("open_video", [&](){
			AssertThat(engine->open("interlace_%03d.bmp"), IsTrue());

			QThread::msleep(FRAME_WAIT_TIME);
		});

		it("default_framerate", [&](){
			AssertThat(engine->open("interlace_%03d.bmp"), IsTrue());

			QThread::msleep(FRAME_WAIT_TIME);

			AssertThat(engine->framePerSecond(), Equals(25.00f));
		});

		it("go_to_01", [&](){
			AssertThat(engine->open("interlace_%03d.bmp"), IsTrue());

			QTest::qWait(FRAME_WAIT_TIME);
			QImage result = view->renderPixmap(64, 64).toImage();
			result.save("result.bmp");

			AssertThat(result == QImage("interlace_000.bmp"), IsTrue());

			engine->clock()->setFrame(20, PhTimeCodeType25);

			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_020.bmp"), IsTrue());

			engine->clock()->setFrame(100, PhTimeCodeType25);

			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_100.bmp"), IsTrue());

			engine->clock()->setFrame(75, PhTimeCodeType25);

			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_075.bmp"), IsTrue());
		});

		it("go_to_02", [&](){
			AssertThat(engine->open("interlace_%03d.bmp"), IsTrue());

			engine->clock()->setFrame(100, PhTimeCodeType25);

			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_100.bmp"), IsTrue());

			engine->clock()->setFrame(99, PhTimeCodeType25);

			PHDEBUG << "second paint";

			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_099.bmp"), IsTrue());

			for(int i = 75; i >= 50; i--) {
				engine->clock()->setFrame(i, PhTimeCodeType25);

				qDebug() << "Set frame :" << i;

				QTest::qWait(FRAME_WAIT_TIME);
				QString name = QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'));
				AssertThat(view->renderPixmap(64, 64).toImage() == QImage(name), IsTrue());
			}
		});

		// This "stress test" cue the video engine at different random location
		it("go_to_03", [&](){
			AssertThat(engine->open("interlace_%03d.bmp"), IsTrue());

			QList<PhFrame> list = QList<PhFrame>() << 183 << 25 << 71 << 59 << 158 << 8 << 137
												   << 32 << 37 << 53 << 133 << 108 << 166 << 134
												   << 110 << 42 << 54 << 107 << 133 << 100 << 169
												   << 70 << 22 << 12 << 176 << 159 << 180 << 55
												   << 113 << 89 << 48 << 187 << 16 << 124 << 153
												   << 90 << 100 << 68 << 96 << 51 << 146 << 154
												   << 115 << 165 << 85 << 83 << 181 << 57 << 86 << 166;


			for(int i = 0; i < list.count(); i++) {
				PhFrame frame = list[i];
				engine->clock()->setFrame(frame, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);
				QString name = QString("interlace_%1.bmp").arg(frame, 3, 10, QChar('0'));
				AssertThat(compareImage(view->renderPixmap(64, 64).toImage(), QImage(name), "go_to_03"), IsTrue());
			}
		});

		it("play", [&](){
			AssertThat(engine->open("interlace_%03d.bmp"), IsTrue());

			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_000.bmp"), IsTrue());

			engine->clock()->setRate(1);
			engine->clock()->elapse(960); // 1 frame at 25 fps

			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_001.bmp"), IsTrue());


			// Play 1 second
			for(int i = 0; i < 25; i++) {
				engine->clock()->elapse(960); // 1 frame at 25 fps
				QTest::qWait(FRAME_WAIT_TIME);
			}

			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_026.bmp"), IsTrue());

			engine->clock()->setRate(-1);
			engine->clock()->elapse(960); // 1 frame at 25 fps
			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_025.bmp"), IsTrue());

			// Play 1 second
			for(int i = 24; i >= 0; i--) {
				engine->clock()->elapse(960); // 1 frame at 25 fps
				QTest::qWait(FRAME_WAIT_TIME);
			}
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_000.bmp"), IsTrue());
		});

		it("deinterlace", [&](){
			// Open the video file in interlaced mode
			engine->open("interlace_%03d.bmp");
			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_000.bmp"), IsTrue());

			//Change mode to deinterlaced
			engine->setDeinterlace(true);
			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("deinterlace_000.bmp"), IsTrue());

			//Move one picture forward
			engine->clock()->setFrame(1, PhTimeCodeType25);
			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("deinterlace_001.bmp"), IsTrue());

			// Go back to interlaced mode
			engine->setDeinterlace(false);
			QTest::qWait(FRAME_WAIT_TIME);
			AssertThat(view->renderPixmap(64, 64).toImage() == QImage("interlace_001.bmp"), IsTrue());
		});

		it("scales", [&](){
			// Open the video file in interlaced mode
			engine->open("interlace_%03d.bmp");
			QTest::qWait(FRAME_WAIT_TIME);

			factor = 2;

			settings->setUseNativeVideoSize(false);

			AssertThat(compareImage(view->renderPixmap(128, 128).toImage(), QImage("interlace_000_scaled.bmp"), "interlace_000_scaled"), IsTrue());
		});

		it("doesn't scale when using native video size", [&](){
			// Open the video file in interlaced mode
			engine->open("interlace_%03d.bmp");
			QTest::qWait(FRAME_WAIT_TIME);

			factor = 2;

			settings->setUseNativeVideoSize(true);

			AssertThat(compareImage(view->renderPixmap(128, 128).toImage(), QImage("interlace_000_native.bmp"), "interlace_000_native"), IsTrue());
		});

		//	it("findMatch(QImage source", [&](){
		//	   for(int i = 0; i < 200; i++) {
		//		if(QImage(QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'))) == source) {
		//			PHDEBUG << "The matching frame is" << QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'));
		//			break;
		//		}
		//	}
		//	for(int i = 0; i < 200; i++) {
		//		if(QImage(QString("deinterlace_%1.bmp").arg(i, 3, 10, QChar('0'))) == source) {
		//			PHDEBUG << "The matching frame is" << QString("deinterlace_%1.bmp").arg(i, 3, 10, QChar('0'));
		//			break;
		//		}
		//	}
	});
});
