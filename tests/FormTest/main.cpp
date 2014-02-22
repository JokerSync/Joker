#include <QApplication>
#include <QFile>

#include "PhTools/PhDebug.h"

#include "FormTestWindow.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	PHDEBUG << __MAC_OS_X_VERSION_MAX_ALLOWED;
	PHDEBUG << __MAC_10_8;

	FormTestSettings settings;
	PHDEBUG << ORG_NAME << APP_NAME << APP_VERSION;
	QApplication a(argc, argv);
	a.setApplicationVersion(APP_VERSION);
	a.setOrganizationName(ORG_NAME);
	a.setOrganizationDomain(ORG_NAME);

	FormTestWindow window(&settings);
	window.processArg(argc, argv);
	window.show();

	return a.exec();
}
