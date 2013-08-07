#include "MainWindow.h"
#include <QApplication>
#include <QtGui>
#include <QtCore>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QWidget w, fenetre;
	fenetre.resize(400,300);
	w.resize(350,250);
	GraphicTestView g(&w, "view");
	g.resize(350,250);

	QGridLayout *gLayout = new QGridLayout;

	gLayout->addWidget(&w, 0, 0);

	fenetre.setLayout(gLayout);
	fenetre.show();

//	w.resize(400,300);
//	g.resize(380,280);
	return a.exec();
}
