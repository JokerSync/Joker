/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QTest>
#include <QWindow>

#include "GraphicStripTest.h"

GraphicStripTest::GraphicStripTest(QWidget *parent) :
	PhGraphicView(parent)
{
	_strip.setSettings(&_settings);
}

bool GraphicStripTest::init()
{
	return _strip.init();
}

void GraphicStripTest::paint()
{
	_strip.draw(0, 0, this->width(), this->height());
}

void GraphicStripTest::testStripDocObject()
{
	this->setGeometry(0, 0, 981, 319);

	PhStripDoc * doc = _strip.doc();
	doc->reset();
	doc->addPeople(new PhPeople("A people"));
	doc->addPeople(new PhPeople("A second people", "red"));

	doc->addObject(new PhStripText(0, doc->peoples().first(), 10000, 1, "Hello"));
	doc->addObject(new PhStripCut(PhStripCut::CrossFade, 5400));
	doc->addObject(new PhStripDetect(PhStripDetect::Off, 0, doc->peoples().first(), 10000, 1));
	doc->addObject(new PhStripLoop(3, 22000));
	doc->addObject(new PhStripText(10000, doc->peoples().last(), 15000, 2, "Hi !"));
	doc->addObject(new PhStripDetect(PhStripDetect::SemiOff, 10000, doc->peoples().last(), 15000, 2));
	doc->changed();

	updateGL();

	QTest::qWait(1000);

	QImage impr(grabFrameBuffer());
	impr.save("graphicStripTestResult.bmp");
	QString expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/graphicStripTest.bmp");
	if(this->windowHandle()->devicePixelRatio() == 2)
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/graphicStripRetinaTest.bmp");
	if(QString(qgetenv("TRAVIS")) == "true")
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/graphicStripTravisTest.bmp");

	QVERIFY2(impr == QImage(expectedFile), PHNQ(expectedFile));
}
