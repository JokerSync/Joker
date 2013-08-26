#include <QApplication>
#include <QtGui>
#include <QtCore>
#include "PhGraphicSolidRect.h"
#include "MainWindow.h"
#include "GraphicTestView.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
//	w.resize(400,300);
	w.show();

	return a.exec();
}
