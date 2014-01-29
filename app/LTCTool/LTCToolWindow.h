#ifndef LTCTOOLWINDOW_H
#define LTCTOOLWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>

#include "PhTools/PhClock.h"
#include "PhSync/PhLtcWriter.h"

namespace Ui {
class LTCToolWindow;
}

class LTCToolWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit LTCToolWindow(QSettings *settings, QWidget *parent = 0);
	~LTCToolWindow();

public slots:

	void updateFrame();

private slots:
	void on_actionSet_TC_In_triggered();

	void on_actionSet_TC_Out_triggered();

	void on_actionPreferences_triggered();

private:
	void setupOutput();
	void updateInfos();
	Ui::LTCToolWindow *ui;
	PhClock *_clock;
	QTimer *_timer;
	QSettings *_settings;
	PhLtcWriter _LTCWriter;



};

#endif // LTCTOOLWINDOW_H
