#include <QApplication>
#include <QFile>

#include "VideoSyncTestWindow.h"
#include "VideoSyncTestSettings.h"

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[])
{
	VideoSyncTestSettings settings;
	QApplication app(argc, argv);

	VideoSyncTestWindow w(&settings);
	w.processArg(argc, argv);
	w.show();

	return app.exec();
}
