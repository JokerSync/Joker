/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PREFERENCESPANEL_H
#define PREFERENCESPANEL_H

#include <QDialog>

namespace Ui {
class PreferencesPanel;
}

/*!
 * \brief LTCTool preferences dialog
 *
 * It allow the user to select the desired input and output.
 */
class PreferencesPanel : public QDialog
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
	explicit PreferencesPanel(QString audioOutput, QString audioInput, QWidget *parent = 0);
	~PreferencesPanel();
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
	Ui::PreferencesPanel *ui;
};

#endif // PREFERENCESPANEL_H
