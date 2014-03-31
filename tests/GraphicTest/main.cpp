#include <QApplication>
#include <QtGui>
#include <QtCore>
#include "GraphicTestWindow.h"
#include "GraphicTestSettings.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	GraphicTestSettings settings;
	QApplication a(argc, argv);
	GraphicTestWindow w(&settings);
	w.show();

	return a.exec();
}
