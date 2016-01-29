/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QThread>

#include "PhTools/PhDebug.h"
#include "PhGraphic/PhGraphicView.h"

#include "PhVideo/PhVideoEngine.h"

#include "VideoSpecSettings.h"

#define FRAME_WAIT_TIME 40

#include "PhSpec.h"
#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("video", [](){
		PhGraphicView *view;
		QString videoFile = "interlace_%03d.bmp";
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
		QString videoCodec = "BMP (Windows and OS/2 bitmap)";
#else
		QString videoCodec = "bmp";
#endif

		VideoSpecSettings *settings;
		PhVideoEngine *engine;
		int factor;

		before_each([&](){
			PhDebug::setLogMask(PHDEBUG_SPEC_MASK | (1 << 9));

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
			AssertThat(engine->codecName().toStdString(), Equals(""));
			AssertThat(engine->length(), Equals(0));
			AssertThat(engine->width(), Equals(0));
			AssertThat(engine->height(), Equals(0));

			AssertThat(engine->open(videoFile), IsTrue());

			AssertThat(engine->codecName().toStdString(), Equals(videoCodec.toStdString()));
			AssertThat(engine->bilinearFiltering(), IsFalse());
			AssertThat(engine->length(), Equals(192000));
			AssertThat(engine->width(), Equals(64));
			AssertThat(engine->height(), Equals(64));

			QThread::msleep(FRAME_WAIT_TIME);
		});

		it("default_framerate", [&](){
			AssertThat(engine->open(videoFile), IsTrue());

			QThread::msleep(FRAME_WAIT_TIME);

			AssertThat(engine->framePerSecond(), Equals(25.00f));
		});

		it("go_to_01", [&](){
			AssertThat(engine->open(videoFile), IsTrue());

			QThread::msleep(FRAME_WAIT_TIME);

			AssertThat(view->compare("interlace_000.bmp"), Equals(0));

			engine->clock()->setFrame(20, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_020.bmp"), Equals(0));

			engine->clock()->setFrame(100, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_100.bmp"), Equals(0));

			engine->clock()->setFrame(75, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_075.bmp"), Equals(0));

			engine->clock()->setFrame(199, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_199.bmp"), Equals(0));

			engine->clock()->setFrame(200, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_199.bmp"), Equals(0));

			engine->clock()->setFrame(-1, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_000.bmp"), Equals(0));
		});

		it("go_to_02", [&](){
			AssertThat(engine->open(videoFile), IsTrue());

			engine->clock()->setFrame(100, PhTimeCodeType25);

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_100.bmp"), Equals(0));

			engine->clock()->setFrame(99, PhTimeCodeType25);

			PHDEBUG << "second paint";

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_099.bmp"), Equals(0));

			for(int i = 75; i >= 50; i--) {
				engine->clock()->setFrame(i, PhTimeCodeType25);

				qDebug() << "Set frame :" << i;

				QThread::msleep(FRAME_WAIT_TIME);
				QString name = QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'));
				AssertThat(view->compare(name), Equals(0));
			}
		});

		// This "stress test" cue the video engine at different random location
		it("go_to_03", [&](){
			AssertThat(engine->open(videoFile), IsTrue());

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

				QThread::msleep(FRAME_WAIT_TIME);
				QString name = QString("interlace_%1.bmp").arg(frame, 3, 10, QChar('0'));
				AssertThat(view->compare(name), Equals(0));
			}
		});

		it("go to interframe", [&]() {
			AssertThat(engine->open(videoFile), IsTrue());

			AssertThat(view->compare("interlace_000.bmp"), Equals(0));

			engine->clock()->setTime(960);
			QThread::msleep(FRAME_WAIT_TIME);

			AssertThat(view->compare("interlace_001.bmp"), Equals(0));

			engine->clock()->setTime(959);
			QThread::msleep(FRAME_WAIT_TIME);

			AssertThat(view->compare("interlace_000.bmp"), Equals(0));
		});

		it("play", [&](){
			AssertThat(engine->open(videoFile), IsTrue());

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_000.bmp"), Equals(0));

			engine->clock()->setRate(1);
			engine->clock()->elapse(960); // 1 frame at 25 fps

			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_001.bmp"), Equals(0));


			// Play 1 second
			for(int i = 0; i < 25; i++) {
				engine->clock()->elapse(960); // 1 frame at 25 fps
				QThread::msleep(FRAME_WAIT_TIME);
			}

			AssertThat(view->compare("interlace_026.bmp"), Equals(0));

			engine->clock()->setRate(-1);
			engine->clock()->elapse(960); // 1 frame at 25 fps
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_025.bmp"), Equals(0));

			// Play 1 second
			for(int i = 24; i >= 0; i--) {
				engine->clock()->elapse(960); // 1 frame at 25 fps
				QThread::msleep(FRAME_WAIT_TIME);
			}
			AssertThat(view->compare("interlace_000.bmp"), Equals(0));
		});

		it("deinterlace", [&](){
			// Open the video file in interlaced mode
			engine->open(videoFile);
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_000.bmp"), Equals(0));

			//Change mode to deinterlaced
			engine->setDeinterlace(true);
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("deinterlace_000.bmp"), Equals(0));

			//Move one picture forward
			engine->clock()->setFrame(1, PhTimeCodeType25);
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("deinterlace_001.bmp"), Equals(0));

			// Go back to interlaced mode
			engine->setDeinterlace(false);
			QThread::msleep(FRAME_WAIT_TIME);
			AssertThat(view->compare("interlace_001.bmp"), Equals(0));
		});

		it("scales", [&](){
			// Open the video file in interlaced mode
			engine->open(videoFile);
			QThread::msleep(FRAME_WAIT_TIME);

			factor = 2;

			settings->setUseNativeVideoSize(false);

			AssertThat(view->compare("interlace_000_scaled.bmp", 128, 128), Equals(0));
		});

		it("doesn't scale when using native video size", [&](){
			// Open the video file in interlaced mode
			engine->open(videoFile);
			QThread::msleep(FRAME_WAIT_TIME);

			factor = 2;

			settings->setUseNativeVideoSize(true);

			AssertThat(view->compare("interlace_000_native.bmp", 128, 128), Equals(0));
		});

		it("handles timestamp", [&](){
			engine->open(videoFile);
			engine->setTimeIn(240000);
			QThread::msleep(FRAME_WAIT_TIME);

			AssertThat(view->compare("interlace_000.bmp"), Equals(0));
			engine->clock()->setTime(240000 + 24000);
			AssertThat(view->compare("interlace_025.bmp"), Equals(0));
			engine->clock()->setTime(0);
			AssertThat(view->compare("interlace_000.bmp"), Equals(0));
			AssertThat(engine->clock()->time(), Equals(0));
		});
	});
});
