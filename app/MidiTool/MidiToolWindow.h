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

#include "PhSync/PhClock.h"

#include "PhMidi/PhMidiTimeCodeReader.h"
#include "PhMidi/PhMidiTimeCodeWriter.h"

#include "MidiToolSettings.h"

namespace Ui {
class MidiToolWindow;
}
/**
 * @brief MidiTool main application window
 *
 * The MidiToolWindow class implements the main screen user interface behaviour:
 * - Display the writer TC in / TC out
 * - Display the writer PhMediaPanel
 * - Display the reader timecode label
 * - Opening application dialog : preferences, set TC in, set TC out
 * - ...
 */
class MidiToolWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * @brief The MidiToolWindow constructor
	 * @param settings The application settings
	 * @param parent
	 */
	explicit MidiToolWindow(MidiToolSettings *settings, QWidget *parent = 0);
	~MidiToolWindow();

private slots:
	/* QT auto slots */
	void on_actionSet_TC_In_triggered();
	void on_actionSet_TC_Out_triggered();
	void on_actionPreferences_triggered();
	void on_writeMtcCheckBox_clicked(bool checked);
	void on_readMtcCheckBox_clicked(bool checked);

	/* custom slots */
	void onGoToTime(PhTime time);
	void onWriterTimeChanged(PhTime time);
	void updateTCTypeSetting(PhTimeCodeType tcType);
	void updateRateSetting(PhRate rate);
	void onReaderTimeChanged(PhTime time);
	void onReaderRateChanged(PhRate rate);
	void updateFpsLabel(PhTimeCodeType tcType);
	void onTick();

	void on_writerLoopPlaybackCheckBox_toggled(bool checked);

private:
	void updateWriterInfoLabel();

	Ui::MidiToolWindow *ui;
	MidiToolSettings *_settings;

	PhMidiTimeCodeReader _mtcReader;
	PhMidiTimeCodeWriter _mtcWriter;

	QTimer _clockTimer;
	int _currentDigit;

	PhFrame _lastFrame;
	PhFrame _frameDelta;
	PhRate _lastRate;
};

#endif // MIDITOOLWINDOW_H
