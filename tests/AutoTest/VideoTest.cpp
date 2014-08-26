/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */
#include <QTest>
#include <QAction>

#include "PhTools/PhDebug.h"
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

void VideoTest::goToTest()
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
	for(int i = 0; i < 25; i++) {
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

void VideoTest::saveBuffer(QString fileName)
{
	QImage image = _view.renderPixmap(64, 64).toImage();
	image.save(fileName);
}
