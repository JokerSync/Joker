#include "MainWindow.h"
#include <QApplication>
#include <QtGui>
#include <QtCore>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.resize(400,300);
	GraphicTestView g(&w);
	g.resize(350,250);


	w.show();

	return a.exec();
}
