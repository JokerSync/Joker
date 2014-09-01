#include <QtGui>
#include <QApplication>

#include "TimecodePlayerWindow.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	TimecodePlayerWindow w;
	w.show();

	return a.exec();
}
