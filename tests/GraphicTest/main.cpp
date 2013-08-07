#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.resize(400,300);
	GraphicTestView g(&w, "view");
	g.resize(250,200);
	w.show();
	return a.exec();
}
