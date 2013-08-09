#include "MainWindow.h"
#include <QApplication>
#include <QtGui>
#include <QtCore>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QWidget w;
	//w.resize(400,300);
	GraphicTestView g(&w);
	//g.resize(350,250);

//	QGridLayout *gLayout = new QGridLayout;
//	gLayout->setMargin(0);
//	gLayout->addWidget(&g, 0, 0);

//	w.setLayout(gLayout);
	w.show();

	return a.exec();
}
