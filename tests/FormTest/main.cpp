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
	FormTestSettings settings;
	PHDEBUG << PH_ORG_NAME << PH_APP_NAME << PH_FULL_VERSION;
	QApplication a(argc, argv);
	PHDEBUG << a.applicationVersion();
	a.setApplicationVersion(PH_FULL_VERSION);
	a.setOrganizationName(PH_ORG_NAME);
	a.setOrganizationDomain(PH_ORG_NAME);

	PHDEBUG << a.applicationVersion();

	FormTestWindow window(&settings);
	window.show();

	return a.exec();
}
