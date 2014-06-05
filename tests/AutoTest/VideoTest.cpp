/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */
#include <QTest>
#include <QAction>

#include "PhTools/PhDebug.h"
#include "PhTools/PhTime.h"
#include "PhTools/PhPictureTools.h"

#include "VideoTest.h"

#define WAIT_TIME 40

VideoTest::VideoTest()
{
	_videoEngine.setSettings(&_settings);
	_view.resize(64,64);

	connect(&_view, &PhGraphicView::paint, [&](int w, int h) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_videoEngine.drawVideo(0, 0, w, h);
	});
}

void VideoTest::openMovieTest()
{
	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );
	_videoEngine.close();
}

void VideoTest::goToTest01() {
	qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";


	_view.show();

	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );

	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_000.bmp"));

	_videoEngine.clock()->setFrame(PhTimeCode::frameFromString("00:00:00:20", PhTimeCodeType25));


	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_020.bmp"));

	_videoEngine.clock()->setFrame(PhTimeCode::frameFromString("00:00:04:00", PhTimeCodeType25));


	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_100.bmp"));

	_videoEngine.clock()->setFrame(PhTimeCode::frameFromString("00:00:03:00", PhTimeCodeType25));


	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_075.bmp"));

	_videoEngine.close();
}

void VideoTest::goToTest02() {
	qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
	qDebug() << "Init graphic _view";


	qDebug() << "Set settings of video engine";


	qDebug() << "show the GV";
	_view.show();

	qDebug() << "open the files";

	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );

	qDebug() << "set the frame";
	_videoEngine.clock()->setFrame(100);

	QTest::qWait(WAIT_TIME);


	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_100.bmp"));

	_videoEngine.clock()->setFrame(99);


	PHDEBUG << "second paint";

	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_099.bmp"));

	for(int i = 75; i >= 50; i--) {
		_videoEngine.clock()->setFrame(i);

		qDebug() << "Set frame :" << i;

		QTest::qWait(WAIT_TIME);
		QString name = QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'));
		QVERIFY2(QImage(_view.grabFrameBuffer()) == QImage(name), PHNQ(name));
	}

	_videoEngine.close();
}

// This "stress test" cue the video engine at different random location
void VideoTest::goToTest03() {
	qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";

	//_videoEngine.setSettings(&_settings);
	_view.show();

	_videoEngine.clock()->setRate(0);


	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );

	QList<int> list = QList<int>() << 183 << 25 << 71 << 59 << 158 << 8 << 137
								   << 32 << 37 << 53 << 133 << 108 << 166 << 134
								   << 110 << 42 << 54 << 107 << 133 << 100 << 169
								   << 70 << 22 << 12 << 176 << 159 << 180 << 55
								   << 113 << 89 << 48 << 187 << 16 << 124 << 153
								   << 90 << 100 << 68 << 96 << 51 << 146 << 154
								   << 115 << 165 << 85 << 83 << 181 << 57 << 86 << 166;
	foreach(int frame, list) {
		_videoEngine.clock()->setFrame(frame);

		QTest::qWait(WAIT_TIME);
		QString name = QString("interlace_%1.bmp").arg(frame, 3, 10, QChar('0'));
		QVERIFY2(QImage(_view.grabFrameBuffer()) == QImage(name), PHNQ(name));
	}

	_videoEngine.close();
}

void VideoTest::playTest() {
	qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";
	//_videoEngine.setSettings(&_settings);

	_view.show();


	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );


	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_000.bmp"));

	_videoEngine.clock()->setRate(1);
	_videoEngine.clock()->tick(25);


	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_001.bmp"));


	// Play 1 second
	for(int i = 0; i < 25; i++) {
		_videoEngine.clock()->tick(25);

		QTest::qWait(WAIT_TIME);
	}

	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_026.bmp"));

	_videoEngine.clock()->setRate(-1);
	_videoEngine.clock()->tick(25);

	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_025.bmp"));

	// Play 1 second
	for(int i = 24; i >= 0; i--) {
		_videoEngine.clock()->tick(25);

		QTest::qWait(WAIT_TIME);
	}
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_000.bmp"));

	_videoEngine.close();
}

void VideoTest::deinterlaceTest() {
	qDebug() << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$";


	_view.show();

	//Open the video file in interlaced mode
	_videoEngine.open("interlace_%03d.bmp");

	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_000.bmp"));

	//Change mode to deinterlaced
	_videoEngine.setDeinterlace(true);

	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("deinterlace_000.bmp"));

	//Move one picture forward
	_videoEngine.clock()->setFrame(1);

	//Go back to interlaced mode
	_videoEngine.setDeinterlace(false);

	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(_view.grabFrameBuffer()) == QImage("interlace_001.bmp"));

}

void VideoTest::saveBuffer(PhGraphicView * _view) {

	QImage test(_view->grabFrameBuffer());
	test.save("result.bmp");
	system("open result.bmp");
}

void VideoTest::findMatch(QImage source) {
	for(int i = 0; i < 200; i++) {
		if(QImage(QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'))) == source) {
			PHDEBUG << "The matching frame is" << QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'));
			break;
		}
	}
	for(int i = 0; i < 200; i++) {
		if(QImage(QString("deinterlace_%1.bmp").arg(i, 3, 10, QChar('0'))) == source) {
			PHDEBUG << "The matching frame is" << QString("deinterlace_%1.bmp").arg(i, 3, 10, QChar('0'));
			break;
		}
	}
}
