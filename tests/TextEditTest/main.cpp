#include "TextEditTestWindow.h"
#include "PhCommonUI/PhFeedbackReporter.h"
#include <QApplication>

#include "PhTools/PhDebug.h"

int main(int argc, char *argv[])
{
	TextEditTestSettings settings;

	QApplication a(argc, argv);
	PhFeedbackReporter f(&settings);

	TextEditTestWindow w(&settings);
	w.processArg(argc, argv);
	w.show();

	if(!settings.exitedNormaly()) {
		f.show();
	}

	settings.setExitedNormaly(false);


	int result = a.exec();
	settings.setExitedNormaly(result == 0);

	return result;
}
