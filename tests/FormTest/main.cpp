#include "MainWindow.h"
#include <QApplication>
#include "PhTools/PhDebug.h"

int main(int argc, char *argv[])
{
	PHDEBUG << ORG_NAME << APP_NAME << APP_VERSION;
	QApplication a(argc, argv);
	a.setApplicationVersion(APP_VERSION);
	a.setOrganizationName(ORG_NAME);
	a.setOrganizationDomain(ORG_NAME);
	MainWindow window;
	window.show();

	QStringList list;
	list.at(1);

    return a.exec();
}
