#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

#include <QTimer>

#include "PhTools/PhTickCounter.h"

namespace Ui {
class SerialTestWindow;
}

class SerialTestWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit SerialTestWindow(QWidget *parent = 0);
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
	QSerialPort* open(QString suffix);
	void closeA();
	void closeB();


	Ui::SerialTestWindow *ui;

	QSerialPort *_serialA, *_serialB;
	QTimer _ctsTimer;
	PhTickCounter _ctsCounter, _timerCounter;
	bool _lastCTS;
};

#endif // MAINWINDOW_H
