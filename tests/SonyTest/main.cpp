#include "MainWindow.h"
#include <QApplication>
#include <QtTest>

#include "PhTools/PhDebug.h"

#include "PhSync/tests/PhSonyControllerTest.h"

int main(int argc, char *argv[])
{
	PhDebug::init(false, true, true, false, false, true, "SonyTest");
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	a.exec();

	PhSonyControllerTest sonyTest;
	//	bool result = sonyTest.testAll();
	//	PHDEBUG << "Run test : " << result;
	return QTest::qExec(&sonyTest);
}
