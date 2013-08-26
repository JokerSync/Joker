#include <QApplication>
#include <QtGui>
#include <QtCore>
#include "PhGraphicSolidRect.h"
#include "QtDesigner.h"
#include "ui_QtDesigner.h"
#include "GraphicTestView.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtDesigner w;
//	w.resize(400,300);
	GraphicTestView g(&w);
	g.resize(800,400);

	QObject::connect(w.getUi()->Play, SIGNAL(clicked()), &g, SLOT(play()));
	QObject::connect(w.getUi()->FastBackward, SIGNAL(clicked()), &g, SLOT(fastBackward()));
	QObject::connect(w.getUi()->FastForward, SIGNAL(clicked()), &g, SLOT(fastForward()));

	w.show();

	return a.exec();
}
