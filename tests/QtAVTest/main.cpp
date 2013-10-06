#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	if(argc > 1)
		w.openFile(argv[1]);

	return a.exec();
}
