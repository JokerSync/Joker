/******************************
* Qt player using libVLC     *
* By protonux                *
*                            *
* Under WTFPL                *
******************************/

#include <QApplication>
#include "player.h"

#ifdef Q_WS_X11
	#include <X11/Xlib.h>
#endif

/**
 * @brief The application main entry point
 * @param argc Command line argument count
 * @param argv Command line argument list
 * @return 0 if the application works well.
 */
int main(int argc, char *argv[]) {
#ifdef Q_WS_X11
	XInitThreads();
#endif

	QApplication app(argc, argv);

	Mwindow player;
	player.show();

	if(argc > 1)
		player.openFile(argv[1]);

	return app.exec();
}
