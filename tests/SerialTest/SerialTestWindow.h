#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#define WINAPI
#define LPCTSTR const char*
#define LPLONG long*
#include <ftd2xx.h>

#include "PhTools/PhTickCounter.h"

#include "SerialTestSettings.h"

namespace Ui {
class SerialTestWindow;
}

class SerialTestWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit SerialTestWindow(SerialTestSettings *settings);
	~SerialTestWindow();

private slots:
	void sendTextA();
	void readTextA();
	void sendTextB();
	void readTextB();

	void on_checkA_toggled(bool checked);

	void on_checkB_toggled(bool checked);
	void checkCTS();

private:
	bool open(FT_HANDLE * serial, int deviceNumber);

	SerialTestSettings *_settings;

	Ui::SerialTestWindow *ui;

	FT_HANDLE _serialA, _serialB;
	QTimer _ctsTimer;
	PhTickCounter _ctsCounter, _timerCounter;
	bool _lastCTS;
};

#endif // MAINWINDOW_H
