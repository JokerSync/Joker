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
	 * @brief The PreferencesDialog constructor
	 * @param settings The application settings.
	 */
	PreferencesDialog(MidiToolSettings *settings);

	/**
	 * @brief The PreferencesDialog destructor
	 */
	~PreferencesDialog();

private slots:
	void updateInputPortEnabledControl();
	void updatePortComboxContent();

private:
	void accept();
	void reject();

	Ui::PreferencesDialog *ui;
	MidiToolSettings *_settings;
	QString _oldMidiOutputPortName, _oldMidiInputPortName, _oldMidiVirtualInputPortName;
};

#endif // PREFERENCESDIALOG_H
