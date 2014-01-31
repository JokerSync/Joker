#ifndef LTCTOOLWINDOW_H
#define LTCTOOLWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include <QSettings>

#include "PhTools/PhClock.h"
#include "PhSync/PhLtcWriter.h"
#include "PhSync/PhLtcReader.h"

namespace Ui {
class LTCToolWindow;
}

class LTCToolWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit LTCToolWindow(QSettings *settings, QWidget *parent = 0);
	~LTCToolWindow();

private slots:
	/* QT auto slots */
	void on_actionSet_TC_In_triggered();
	void on_actionSet_TC_Out_triggered();
	void on_actionPreferences_triggered();

	/** If the application loops the LTC,
	 *  Reset clock to tcIn if tc > tcOut
	 */
	void onFrameChanged(PhFrame, PhTimeCodeType);
	void onSlaveFrameChanged(PhFrame,PhTimeCodeType);

private:
	void setupOutput();
	void updateInfos();
	Ui::LTCToolWindow *ui;
	PhClock *_writingClock;
	QSettings *_settings;
	PhLtcWriter _LTCWriter;
	PhLtcReader _LTCReader;

	void setupInput();
};

#endif // LTCTOOLWINDOW_H
