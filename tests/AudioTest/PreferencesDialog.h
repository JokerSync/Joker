/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

namespace Ui {
class PreferencesDialog;
}

/*!
 * \brief AudioTest preferences dialog
 *
 * It allow the user to select the desired input and output.
 */
class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
	 * \brief Preferences Panel
	 *
	 * Create a modal QDialog which prompt the user for devices.
	 *
	 * \param audioOutput	The current output device.
	 * \param audioInput	The current input device.
	 * \param parent		The parent.
	 */
	explicit PreferencesDialog(QString audioOutput, QString audioInput, QWidget *parent = 0);
	~PreferencesDialog();
	/*!
	 * \brief selectedAudioOutput
	 * \return The selected output device
	 */
	QString selectedAudioOutput();
	/*!
	 * \brief selectedAudioInput
	 * \return The selected input device
	 */
	QString selectedAudioInput();

private:
	Ui::PreferencesDialog *ui;
};

#endif // PREFERENCESDIALOG_H
