/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef LTCTOOLWINDOW_H
#define LTCTOOLWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>

#include "PhSync/PhClock.h"
#include "PhLtc/PhLtcWriter.h"
#include "PhLtc/PhLtcReader.h"

#include "LTCToolSettings.h"

namespace Ui {
class LTCToolWindow;
}
/*!
 * \brief LTCTool main application window
 *
 * The LTCToolWindow class implements the main screen user interface behaviour:
 * - Display the generator TC in / TC out
 * - Display the generator PhMediaPanel
 * - Display the reader timecode label
 * - Opening application dialog : preferences, set TC in, set TC out
 * - Connect the application modules to the interface: PhLtcWriter andPhLtcReader
 */
class LTCToolWindow : public QMainWindow
{
	Q_OBJECT

public:
	/*!
	 * \brief The LTCToolWindow constructor
	 * \param settings The application settings
	 * \param parent
	 */
	explicit LTCToolWindow(LTCToolSettings *settings, QWidget *parent = 0);
	~LTCToolWindow();

private slots:
	/* QT auto slots */
	void on_actionSet_TC_In_triggered();
	void on_actionSet_TC_Out_triggered();
	void on_actionPreferences_triggered();

	/**
	 * If the application loops the LTC,
	 * reset clock to tcIn if tc > tcOut
	 */
	void onWriterTimeChanged(PhTime time);
	void onWriterTimeCodeTypeChanged(PhTimeCodeType tcType);
	void onReaderTimeChanged(PhTime);
	void onReaderRateChanged(PhRate);
	void updateReaderInfo();

	void on_generateCheckBox_clicked(bool checked);

	void on_readCheckBox_clicked(bool checked);

	void onAudioProcessed(int minLevel, int maxLevel);

private:
	void setupOutput();
	void updateInOutInfoLabel();
	Ui::LTCToolWindow *ui;
	LTCToolSettings *_settings;
	PhTimeCodeType _writerTimeCodeType;
	PhClock *_writingClock;
	PhLtcWriter _ltcWriter;
	PhLtcReader _ltcReader;

	PhTime _lastTime;
	PhTime _timeDelta;
	PhRate _lastRate;

	void setupInput();
};

#endif // LTCTOOLWINDOW_H
