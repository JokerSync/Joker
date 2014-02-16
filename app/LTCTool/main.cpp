/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "LTCToolWindow.h"
#include <QApplication>

#include <QSettings>

int main(int argc, char *argv[])
{
	QSettings settings("Phonations", APP_NAME);
	QSettings::setDefaultFormat(QSettings::NativeFormat);

	QApplication a(argc, argv);
	LTCToolWindow w(&settings);
	w.show();

	return a.exec();
}
