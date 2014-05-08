/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */
#include <QTest>
#include <QAction>

#include "PhTools/PhDebug.h"
#include "PhGraphic/PhGraphicView.h"
#include "PhTools/PhTime.h"

#include "VideoTest.h"

#define WAIT_TIME 40

VideoTest::VideoTest(QWidget *parent)
	: PhGraphicView( NULL),
	_videoEngine(false)
{
	resize(64, 64);
	_videoEngine.setSettings(&_settings);
}

void VideoTest::openMovieTest()
{
	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );
	_videoEngine.close();
}

void VideoTest::goToTest() {
	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );

	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_000.bmp"));

	_videoEngine.clock()->setFrame(PhTimeCode::frameFromString("00:00:00:20", PhTimeCodeType25));

	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_020.bmp"));

	_videoEngine.clock()->setFrame(PhTimeCode::frameFromString("00:00:04:00", PhTimeCodeType25));

	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_100.bmp"));

	_videoEngine.clock()->setFrame(PhTimeCode::frameFromString("00:00:03:00", PhTimeCodeType25));

	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_075.bmp"));

	_videoEngine.close();
}

void VideoTest::playTest() {
	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );

	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_000.bmp"));

	_videoEngine.clock()->setRate(1);
	_videoEngine.clock()->tick(25);

	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_001.bmp"));


	// Play 1 second
	for(int i = 0; i < 25; i++) {
		_videoEngine.clock()->tick(25);
		updateGL();
		QTest::qWait(WAIT_TIME);
	}

	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_026.bmp"));

	_videoEngine.clock()->setRate(-1);
	_videoEngine.clock()->tick(25);
	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_025.bmp"));

	// Play 1 second
	for(int i = 0; i < 25; i++) {
		_videoEngine.clock()->tick(25);
		updateGL();
		QTest::qWait(WAIT_TIME);
	}
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_000.bmp"));

	_videoEngine.close();
}

void VideoTest::deinterlaceTest() {
	//Open the video file in interlaced mode
	_videoEngine.open("interlace_%03d.bmp");
	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_000.bmp"));

	//Change mode to deinterlaced
	_videoEngine.setDeinterlace(true);
	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("deinterlace_000.bmp"));

	//Move one picture forward
	_videoEngine.clock()->setFrame(PhTimeCode::frameFromString("00:00:00:01", PhTimeCodeType25));

	//Go back to interlaced mode
	_videoEngine.setDeinterlace(false);
	updateGL();
	QTest::qWait(WAIT_TIME);
	QVERIFY(QImage(grabFrameBuffer()) == QImage("interlace_001.bmp"));

}



void VideoTest::paint() {
	_videoEngine.drawVideo(0, 0, width(), height());
}

void VideoTest::saveBuffer() {
	QImage test(grabFrameBuffer());
	test.save("result.bmp");
	system("open result.bmp");
}
