#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>

#include "PhTools/PhTickCounter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void sendTextA();
	void readTextA();
	void sendTextB();
	void readTextB();

	void on_checkA_toggled(bool checked);

	void on_checkB_toggled(bool checked);
	void checkCTS();

private:
	bool open(QSerialPort * serial, QString suffix);
	void closeA();
	void closeB();


	Ui::MainWindow *ui;

	QSerialPort _serialA, _serialB;
	QTimer _ctsTimer;
	PhTickCounter _ctsCounter, _timerCounter;
	bool _lastCTS;
};

#endif // MAINWINDOW_H
