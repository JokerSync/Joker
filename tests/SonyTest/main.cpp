#include "MainWindow.h"
#include <QApplication>

#include "PhTools/PhDebug.h"

#include "PhSync/tests/PhSonyControllerTest.h"

int main(int argc, char *argv[])
{
	PhDebug::init(false, true, true);
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

//	PhSonyControllerTest sonyTest(NULL);
//	bool result = sonyTest.testAll();
//	qDebug() << "Run test : " << result;

	return a.exec();
}
