/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QSignalSpy>

#include "PhTools/PhDebug.h"
#include "PhGraphic/PhGraphicView.h"

#include "PhVideo/PhVideoEngine.h"

#include "VideoSpecSettings.h"

#define FRAME_WAIT_TIME 40
#define OPEN_WAIT_TIME 10000
#define DECODE_WAIT_TIME 50

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("engine", [](){
		PhGraphicView *view;
		VideoSpecSettings *settings;
		PhVideoEngine *engine;
		QSignalSpy *openSpy;
		QSignalSpy *decodeSpy;
		int factor;
		PhTime offset;

		before_each([&](){
			PhDebug::setLogMask(PHDEBUG_SPEC_MASK | (1 << 9)
//								| (1 << 24)
//								| 1
								);

			view = new PhGraphicView(64, 64);
			settings = new VideoSpecSettings();
			engine = new PhVideoEngine(settings);
			openSpy = new QSignalSpy(engine, &PhVideoEngine::opened);
			decodeSpy = new QSignalSpy(engine, &PhVideoEngine::newFrameDecoded);

			// This is just for opengl initialization
//			view->show();
			view->renderPixmap(64, 64);

			engine->setBilinearFiltering(false);

			factor = 1;
			offset = 0;

			QObject::connect(view, &PhGraphicView::paint, [&](int w, int h) {
				engine->drawVideo(0, 0, factor * w, factor * h, offset);
			});
		});

		after_each([&](){
			engine->close();

			delete decodeSpy;
			delete openSpy;
			delete engine;
			delete settings;
			delete view;
		});

		it("is empty", [&](){
			AssertThat(engine->codecName(), Equals(""));
			AssertThat(engine->length(), Equals(0));
			AssertThat(engine->width(), Equals(0));
			AssertThat(engine->height(), Equals(0));
		});

		describe("bmp", [&](){

			before_each([&](){
				AssertThat(engine->open("interlace_%03d.bmp"), IsTrue());
				AssertThat(openSpy->wait(OPEN_WAIT_TIME), IsTrue());
				QTest::qWait(FRAME_WAIT_TIME);
			});

			it("open", [&](){
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
				AssertThat(engine->codecName(), Equals("BMP (Windows and OS/2 bitmap)"));
#else
				AssertThat(engine->codecName(), Equals("bmp"));
#endif

				AssertThat(engine->bilinearFiltering(), IsFalse());
				AssertThat(engine->length(), Equals(192000));
				AssertThat(engine->width(), Equals(64));
				AssertThat(engine->height(), Equals(64));
				AssertThat(engine->timeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(engine->framePerSecond(), Equals(25.00f));
				AssertThat(engine->timeIn(), Equals(0));
			});

			it("go_to_01", [&](){
				AssertThat(view->compare("interlace_000.bmp"), Equals(0));

				engine->clock()->setFrame(20, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_020.bmp"), Equals(0));

				engine->clock()->setFrame(100, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_100.bmp"), Equals(0));

				engine->clock()->setFrame(75, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_075.bmp"), Equals(0));

				engine->clock()->setFrame(199, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_199.bmp"), Equals(0));

				engine->clock()->setFrame(200, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_199.bmp"), Equals(0));

				engine->clock()->setFrame(-1, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_000.bmp"), Equals(0));
			});

			it("go_to_02", [&](){
				engine->clock()->setFrame(100, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_100.bmp"), Equals(0));

				engine->clock()->setFrame(99, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_099.bmp"), Equals(0));

				for(int i = 75; i >= 50; i--) {
					engine->clock()->setFrame(i, PhTimeCodeType25);

					QTest::qWait(FRAME_WAIT_TIME);
					QString name = QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'));

					AssertThat(view->compare(name), Equals(0));
				}
			});

			// This "stress test" cue the video engine at different random location
			it("go_to_03", [&](){
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
					AssertThat(view->compare(name), Equals(0));
				}
			});

			it("go to interframe", [&]() {
				AssertThat(view->compare("interlace_000.bmp"), Equals(0));

				engine->clock()->setTime(960);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_001.bmp"), Equals(0));

				engine->clock()->setTime(959);

				AssertThat(view->compare("interlace_000.bmp"), Equals(0));
			});

			it("play", [&](){
				AssertThat(view->compare("interlace_000.bmp"), Equals(0));

				engine->clock()->setRate(1);
				engine->clock()->elapse(960); // 1 frame at 25 fps

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_001.bmp"), Equals(0));


				// Play 1 second
				for(int i = 2; i <= 26; i++) {
					engine->clock()->elapse(960); // 1 frame at 25 fps

					QTest::qWait(FRAME_WAIT_TIME);

					QString name = QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'));

					AssertThat(view->compare(name, 0), Equals(0));
				}

				engine->clock()->setRate(-1);
				engine->clock()->elapse(960); // 1 frame at 25 fps

				AssertThat(view->compare("interlace_025.bmp"), Equals(0));

				// Play 1 second
				for(int i = 24; i >= 0; i--) {
					engine->clock()->elapse(960); // 1 frame at 25 fps

					QTest::qWait(FRAME_WAIT_TIME);

					QString name = QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'));
					AssertThat(view->compare(name, 0), Equals(0));
				}
			});

			it("deinterlace", [&](){
				AssertThat(view->compare("interlace_000.bmp"), Equals(0));

				//Change mode to deinterlaced
				engine->setDeinterlace(true);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("deinterlace_000.bmp"), Equals(0));

				//Move one picture forward
				engine->clock()->setFrame(1, PhTimeCodeType25);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("deinterlace_001.bmp"), Equals(0));

				// Go back to interlaced mode
				engine->setDeinterlace(false);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_001.bmp"), Equals(0));
			});

			it("scales", [&](){
				factor = 2;

				settings->setUseNativeVideoSize(false);

				AssertThat(view->compare("interlace_000_scaled.bmp", 0, 128, 128), Equals(0));
			});

			it("doesn't scale when using native video size", [&](){
				factor = 2;

				settings->setUseNativeVideoSize(true);

				AssertThat(view->compare("interlace_000_native.bmp", 0, 128, 128), Equals(0));
			});

			it("handles timestamp", [&](){
				engine->setTimeIn(240000);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_000.bmp"), Equals(0));
				engine->clock()->setTime(240000 + 96000);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_100.bmp"), Equals(0));
				engine->clock()->setTime(0);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_000.bmp"), Equals(0));
				AssertThat(engine->clock()->time(), Equals(0));
			});

			it("buffer", [&](){
				// engine decode 5 frames head
				while(decodeSpy->count() < 5) {
					AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsTrue());
				}
				AssertThat(decodeSpy->count(), Equals(5));
				AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsFalse());

				// move one frame head decode one frame more
				engine->clock()->setFrame25(1);
				AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsTrue());
				AssertThat(decodeSpy->count(), Equals(6));
				AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsFalse());

				engine->clock()->setFrame25(100);
				// engine decode 5 frames head
				while (decodeSpy->count() < 11) {
					AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsTrue());
				}
				AssertThat(decodeSpy->count(), Equals(11));
				AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsFalse());
			});

			it("has a frame pool", [&](){
				// Add 5 first frame to the pool
				while (decodeSpy->count() < 5) {
					AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsTrue());
				}
				AssertThat(decodeSpy->count(), Equals(5));

				AssertThat(engine->decodedFramePool().count(), Equals(5));

				// Add 50 other frame to the pool
				for(int frame = 1; frame <= 50; frame++) {
					engine->clock()->setFrame25(frame);
					AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsTrue());
					QTest::qWait(FRAME_WAIT_TIME);
					AssertThat(decodeSpy->count(), Equals(frame + 5));
					AssertThat(engine->decodedFramePool().count(), Equals(frame + 5));
				}

				// Add another frame without increasing the pool size
				engine->clock()->setFrame25(51);
				AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsTrue());
				AssertThat(decodeSpy->count(), Equals(56));
				AssertThat(engine->decodedFramePool().count(), Equals(55));

				// Jump to a far location clear the pool
				engine->clock()->setFrame25(120);
				while(decodeSpy->count() < 61) {
					AssertThat(decodeSpy->wait(DECODE_WAIT_TIME), IsTrue());
				}
				AssertThat(decodeSpy->count(), Equals(61));
				AssertThat(engine->decodedFramePool().count(), Equals(5));
			});

			it("handles offset", [&]() {
				offset = 960 * 2;
				engine->clock()->setFrame25(10);

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_012.bmp"), Equals(0));
			});
		});

		describe("x264", [&](){
			int threshold = 64 * 64 * 32; // allow high threshold due to compression

			before_each([&](){
				AssertThat(engine->open("interlace_x264_25fps.mkv"), IsTrue());
				AssertThat(openSpy->wait(OPEN_WAIT_TIME), IsTrue());
				QTest::qWait(FRAME_WAIT_TIME);
			});

			it("read properties", [&](){
				AssertThat(engine->codecName().toStdString(), Equals("H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10"));

				AssertThat(engine->bilinearFiltering(), IsFalse());
				AssertThat(engine->length(), Equals(192000));
				AssertThat(engine->width(), Equals(64));
				AssertThat(engine->height(), Equals(64));
				AssertThat(engine->timeCodeType(), Equals(PhTimeCodeType25));
				AssertThat(engine->framePerSecond(), Equals(25.00f));
				AssertThat(engine->timeIn(), Equals(0));
			});

			it("play x264", [&](){
				AssertThat(view->compare("interlace_000.bmp", threshold), IsLessThan(threshold));

				engine->clock()->setRate(1);
				engine->clock()->elapse(960); // 1 frame at 25 fps

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_001.bmp", threshold), IsLessThan(threshold));

				// Play 1 second
				for(int frame = 2; frame < 27; frame++) {
					engine->clock()->elapse(960); // 1 frame at 25 fps
					QTest::qWait(FRAME_WAIT_TIME);
					QString name = QString("interlace_%1.bmp").arg(frame, 3, 10, QChar('0'));
					AssertThat(view->compare(name, threshold), IsLessThan(threshold));
				}

				engine->clock()->setRate(-1);

				engine->clock()->elapse(960); // 1 frame at 25 fps

				QTest::qWait(FRAME_WAIT_TIME);

				AssertThat(view->compare("interlace_025.bmp", threshold), IsLessThan(threshold));

				// Play 1 second
				for(int frame = 24; frame >= 0; frame--) {
					engine->clock()->elapse(960); // 1 frame at 25 fps

					QTest::qWait(FRAME_WAIT_TIME);

					QString name = QString("interlace_%1.bmp").arg(frame, 3, 10, QChar('0'));
					AssertThat(view->compare(name, threshold), IsLessThan(threshold));
				}
			});

//			it("go to before keyframe", [&](){
//				AssertThat(paintSpy->wait(PAINT_WAIT_TIME), IsTrue());

//				// go to the next key frame
//				engine->clock()->setFrame25(128);
//				AssertThat(paintSpy->wait(PAINT_WAIT_TIME), IsTrue());

//				AssertThat(view->compare("interlace_128.bmp", threshold), IsLessThan(threshold));

//				// go to the frame right before the key frame
//				engine->clock()->setFrame25(127);
//				AssertThat(paintSpy->wait(PAINT_WAIT_TIME), IsTrue());
//				AssertThat(view->compare("interlace_127.bmp", threshold), IsLessThan(threshold));

//				// go to two frames before the key frame
//				engine->clock()->setFrame25(126);
//				AssertThat(paintSpy->wait(PAINT_WAIT_TIME), IsTrue());
//				AssertThat(view->compare("interlace_126.bmp", threshold), IsLessThan(threshold));
//			});
		});
	});
});
