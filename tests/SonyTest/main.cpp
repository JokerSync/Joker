#include "MainWindow.h"
#include <QApplication>

#include "PhSync/tests/PhSonyControllerTest.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

//	PhSonyControllerTest sonyTest(NULL);
//	bool result = sonyTest.testAll();
//	qDebug() << "Run test : " << result;

	return a.exec();
}
