#include "LTCToolWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	LTCToolWindow w;
	w.show();

	return a.exec();
}
