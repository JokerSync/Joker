#include "TextEditTestWindow.h"
#include <QApplication>

#include "PhTools/PhDebug.h"

int main(int argc, char *argv[])
{
	TextEditTestSettings settings;

	QApplication a(argc, argv);

	TextEditTestWindow w(&settings);
	w.processArg(argc, argv);
	w.show();

	return a.exec();
}
