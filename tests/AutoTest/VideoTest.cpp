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

VideoTest::VideoTest(QWidget *parent)
	: PhGraphicView( NULL),
	_videoEngine(false)
{
	resize(40, 40);

}

void VideoTest::openMovieTest()
{
	QVERIFY(_videoEngine.open(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/file_%03d.bmp") );
	_videoEngine.close();
}

void VideoTest::goToTest() {
	QVERIFY(_videoEngine.open(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/file_%03d.bmp") );

	paint();
	QTest::qWait(100);
	QVERIFY(QImage(grabFrameBuffer()) == QImage(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/file_000.bmp"));

	_videoEngine.clock()->setFrame(PhTimeCode::frameFromString("00:00:00:20", PhTimeCodeType25));

	paint();
	QTest::qWait(100);
	QVERIFY(QImage(grabFrameBuffer()) == QImage(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/file_020.bmp"));

	_videoEngine.clock()->setFrame(PhTimeCode::frameFromString("00:00:04:00", PhTimeCodeType25));

	paint();
	QTest::qWait(100);
	QVERIFY(QImage(grabFrameBuffer()) == QImage(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/file_100.bmp"));
}

void VideoTest::paint() {
	_videoEngine.drawVideo(0, 0, 40, 40);
}
