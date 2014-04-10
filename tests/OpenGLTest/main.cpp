#include <QApplication>

#include "OpenGLTestWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenGLTestWindow w;
	w.show();

	return a.exec();
}
