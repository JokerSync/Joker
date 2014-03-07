#include "SerialTestWindow.h"
#include <QApplication>

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	SerialTestWindow w;
	w.show();

	return a.exec();
}
