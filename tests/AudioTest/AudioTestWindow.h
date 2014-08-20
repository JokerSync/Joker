/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef AUDIOTESTWINDOW_H
#define AUDIOTESTWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include <QSettings>

#include "PhSync/PhClock.h"

#include "PhAudio/PhAudioInput.h"
#include "AudioTestWriter.h"

namespace Ui {
class AudioTestWindow;
}
/*!
 * \brief AudioTest main application window
 *
 * The AudioTestWindow class implements the main screen user interface behaviour:
 * - Generate sound
 * - Read sound
 */
class AudioTestWindow : public QMainWindow
{
	Q_OBJECT

public:
	/*!
	 * \brief The AudioTestWindow constructor
	 * \param settings The application settings
	 * \param parent
	 */
	explicit AudioTestWindow(QSettings *settings, QWidget *parent = 0);
	~AudioTestWindow();

private slots:
	/* QT auto slots */
	void on_actionPreferences_triggered();

	void on_generateCheckBox_clicked(bool checked);

	void on_readCheckBox_clicked(bool checked);

	void onAudioProcessed(int minLevel, int maxLevel);
private:
	void setupOutput();
	void setupInput();

	Ui::AudioTestWindow *ui;
	QSettings *_settings;
	AudioTestWriter _audioWriter;
	PhAudioInput _audioReader;

	PhFrame _lastFrame;
	PhFrame _frameDelta;

};

#endif // AUDIOTESTWINDOW_H
