#include <QApplication>

#include "OpenGLTestWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	OpenGLTestWindow w;
	w.show();

	if(argc > 1)
		w.loadFile(argv[1]);

	return a.exec();
}
