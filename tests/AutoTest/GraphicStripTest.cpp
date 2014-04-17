/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTest>
#include <QDate>
#include "GraphicStripTest.h"

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhGraphicStrip/PhGraphicStripView.h"

GraphicStripTest::GraphicStripTest(QObject *parent) :
	QObject(parent)
{
}

void GraphicStripTest::testStripDocObject()
{
	PhGraphicStripView view;
	GraphicStripTestSettings settings;
	settings.setStripHeight(1);
	settings.setTextFontFile(QCoreApplication::applicationDirPath() + "/SWENSON.TTF");
	view.setStripSettings(&settings);

	view.setMinimumSize(QSize(981, 319));
	view.setMaximumSize(QSize(981, 319));
	view.show();


	PhStripDoc * doc = view.strip()->doc();
	doc->reset();
	doc->addPeople(new PhPeople("A people"));
	doc->addPeople(new PhPeople("A second people", "red"));

	doc->addObject(new PhStripText(0, doc->peoples().first(), 10000, 1, "Hello"));
	doc->addObject(new PhStripCut(PhStripCut::CrossFade, 5400));
	doc->addObject(new PhStripDetect(PhStripDetect::Off, 0, doc->peoples().first(), 10000, 1));
	doc->addObject(new PhStripLoop(3, 22000));
	doc->addObject(new PhStripText(10000, doc->peoples().last(), 15000, 2, "Hi !"));
	doc->addObject(new PhStripDetect(PhStripDetect::SemiOff, 10000, doc->peoples().last(), 15000, 2));



	view.strip()->draw(0, 0, 981, 319);
	QImage impr(view.grabFrameBuffer());
	QDateTime * d = new QDateTime();
	QString filename = "result" + d->currentDateTime().toString("hh:mm:ss:zzz") +".bmp";
	QVERIFY(impr.save(filename));
	QString cmd;
	cmd += "curl --form \"file=@" + filename + "\" http://phonations.com/feedback/serv.php";

	system(cmd.toStdString().c_str());
	QString expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/test1.bmp");
	if(view.windowHandle()->devicePixelRatio() == 2)
		expectedFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + QString("/test2.bmp");
	QImage expectedImage = QImage(expectedFile);

	int imgWidth = expectedImage.width();
	int imgHeight = expectedImage.height();
	int nbDiffPixels = 0;
	for(int i = 1; i < imgWidth; i++) {
		for(int j = 1; j < imgHeight; j++) {
			if(QColor(expectedImage.pixel(i, j)).name() != QColor(impr.pixel(i, j)).name()) {
				nbDiffPixels++;
			}
		}
	}

	// Assuming that getting less than 0,1% of diff is the same
	// file. See http://stackoverflow.com/q/23136336/2307070
	QVERIFY(nbDiffPixels < imgHeight * imgWidth / 1000);
}
