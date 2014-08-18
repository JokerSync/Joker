/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include "MidiToolSettings.h"

namespace Ui {
class PreferencesDialog;
}

/**
 * @brief MidiTool preferences dialog
 *
 * It allow the user to select the desired input and output.
 */
class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief Preferences Panel
	 *
	 * Create a modal QDialog which prompt the user for devices.
	 *
	 * @param settings The application settings.
	 */
	PreferencesDialog(MidiToolSettings *settings);
	~PreferencesDialog();

protected:
	void accept();
	void reject();

private:
	Ui::PreferencesDialog *ui;
	MidiToolSettings *_settings;
	QString _oldMidiInputPortName, _oldMidiOutputPortName;
};

#endif // PREFERENCESDIALOG_H
