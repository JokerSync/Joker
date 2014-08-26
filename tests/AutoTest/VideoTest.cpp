/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */
#include <QTest>
#include <QAction>

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"
#include "PhGraphic/PhGraphicView.h"
#include "PhSync/PhTime.h"

#include "VideoTest.h"

#define FRAME_WAIT_TIME 40

VideoTest::VideoTest() : _videoEngine(&_settings),
	_view(64, 64)
{
	connect(&_view, &PhGraphicView::paint, this, &VideoTest::paint);
	_view.show();
}

void VideoTest::paint(int width, int height)
{
	_videoEngine.drawVideo(0, 0, width, height);
}

void VideoTest::openMovieTest()
{
	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );
	_videoEngine.close();
}

void VideoTest::goToTest01()
{
	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );

	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_000.bmp"));

	_videoEngine.clock()->setFrame(20, PhTimeCodeType25);

	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_020.bmp"));

	_videoEngine.clock()->setFrame(100, PhTimeCodeType25);

	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_100.bmp"));

	_videoEngine.clock()->setFrame(75, PhTimeCodeType25);

	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_075.bmp"));

	_videoEngine.close();
}

void VideoTest::goToTest02()
{
	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );

	qDebug() << "set the frame";
	_videoEngine.clock()->setFrame(100, PhTimeCodeType25);

	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_100.bmp"));

	_videoEngine.clock()->setFrame(99, PhTimeCodeType25);


	PHDEBUG << "second paint";

	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_099.bmp"));

	for(int i = 75; i >= 50; i--) {
		_videoEngine.clock()->setFrame(i, PhTimeCodeType25);

		qDebug() << "Set frame :" << i;

		QTest::qWait(FRAME_WAIT_TIME);
		QString name = QString("interlace_%1.bmp").arg(i, 3, 10, QChar('0'));
		QVERIFY2(_view.renderPixmap(64, 64).toImage() == QImage(name), PHNQ(name));
	}

	_videoEngine.close();
}

// This "stress test" cue the video engine at different random location
void VideoTest::goToTest03()
{
	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );

	QList<int> list = QList<int>() << 183 << 25 << 71 << 59 << 158 << 8 << 137
	                               << 32 << 37 << 53 << 133 << 108 << 166 << 134
	                               << 110 << 42 << 54 << 107 << 133 << 100 << 169
	                               << 70 << 22 << 12 << 176 << 159 << 180 << 55
	                               << 113 << 89 << 48 << 187 << 16 << 124 << 153
	                               << 90 << 100 << 68 << 96 << 51 << 146 << 154
	                               << 115 << 165 << 85 << 83 << 181 << 57 << 86 << 166;
	foreach(int frame, list) {
		_videoEngine.clock()->setFrame(frame, PhTimeCodeType25);

		QTest::qWait(FRAME_WAIT_TIME);
		QString name = QString("interlace_%1.bmp").arg(frame, 3, 10, QChar('0'));
		QVERIFY2(_view.renderPixmap(64, 64).toImage() == QImage(name), PHNQ(name));
	}

	_videoEngine.close();
}

void VideoTest::playTest()
{
	QVERIFY(_videoEngine.open("interlace_%03d.bmp") );

	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_000.bmp"));

	_videoEngine.clock()->setRate(1);
	_videoEngine.clock()->tick(25);

	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_001.bmp"));


	// Play 1 second
	for(int i = 0; i < 25; i++) {
		_videoEngine.clock()->tick(25);
		QTest::qWait(FRAME_WAIT_TIME);
	}

	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_026.bmp"));

	_videoEngine.clock()->setRate(-1);
	_videoEngine.clock()->tick(25);
	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_025.bmp"));

	// Play 1 second
	for(int i = 24; i >= 0; i--) {
		_videoEngine.clock()->tick(25);
		QTest::qWait(FRAME_WAIT_TIME);
	}
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_000.bmp"));

	_videoEngine.close();
}

void VideoTest::deinterlaceTest()
{
	//Open the video file in interlaced mode
	_videoEngine.open("interlace_%03d.bmp");
	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_000.bmp"));

	//Change mode to deinterlaced
	_videoEngine.setDeinterlace(true);
	//Disable bilinear filtering for the test
	_videoEngine.setBilinearFiltering(false);
	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("deinterlace_000.bmp"));

	//Re-enable bilinear filtering
	_videoEngine.setBilinearFiltering(true);

	//Move one picture forward
	_videoEngine.clock()->setFrame(1, PhTimeCodeType25);

	//Go back to interlaced mode
	_videoEngine.setDeinterlace(false);
	QTest::qWait(FRAME_WAIT_TIME);
	QVERIFY(_view.renderPixmap(64, 64).toImage() == QImage("interlace_001.bmp"));
}

void VideoTest::saveBuffer(QString fileName) {
	QImage test = _view.renderPixmap(64,64).toImage();
	test.save(fileName);
	system(PHNQ(QString("open %0").arg(fileName)));
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
