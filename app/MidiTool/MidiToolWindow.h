/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef MIDITOOLWINDOW_H
#define MIDITOOLWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>

#include "PhTools/PhClock.h"

#include "PhMidi/PhMidiInput.h"
#include "PhMidi/PhMidiOutput.h"

#include "MidiToolSettings.h"

namespace Ui {
class MidiToolWindow;
}
/*!
 * \brief MidiTool main application window
 *
 * The MidiToolWindow class implements the main screen user interface behaviour:
 * - Display the generator TC in / TC out
 * - Display the generator PhMediaPanel
 * - Display the reader timecode label
 * - Opening application dialog : preferences, set TC in, set TC out
 * - ...
 */
class MidiToolWindow : public QMainWindow
{
	Q_OBJECT

public:
	/*!
	 * \brief The MidiToolWindow constructor
	 * \param settings The application settings
	 * \param parent
	 */
	explicit MidiToolWindow(MidiToolSettings *settings, QWidget *parent = 0);
	~MidiToolWindow();

private slots:
	/* QT auto slots */
	void on_actionSet_TC_In_triggered();
	void on_actionSet_TC_Out_triggered();
	void on_actionPreferences_triggered();

	void onFrameChanged(PhFrame, PhTimeCodeType);
	void onTC(int hh, int mm, int ss, int ff, PhTimeCodeType tcType);
	void onSlaveRateChanged(PhRate rate);
	void updateSlaveInfo();

	void on_generateCheckBox_clicked(bool checked);

	void on_readCheckBox_clicked(bool checked);

	void onAudioProcessed(int minLevel, int maxLevel);

private:
	void setupOutput();
	void updateInfos();
	Ui::MidiToolWindow *ui;
	MidiToolSettings *_settings;

	PhClock _writingClock;
	PhMidiInput _midiInput;
	PhMidiOutput _midiOutput;

	PhFrame _lastFrame;
	PhFrame _frameDelta;
	PhRate _lastRate;

	void setupInput();
};

#endif // MIDITOOLWINDOW_H
