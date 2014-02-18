#include "MainWindow.h"
#include <QApplication>
#include "PhTools/PhDebug.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	PHDEBUG << ORG_NAME << APP_NAME << APP_VERSION;
	QApplication a(argc, argv);
	a.setApplicationVersion(APP_VERSION);
	a.setOrganizationName(ORG_NAME);
	a.setOrganizationDomain(ORG_NAME);
	MainWindow window;
	window.show();

	return a.exec();
}
