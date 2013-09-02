#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

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

private:
	bool openA();
	void closeA();
	bool openB();
	void closeB();

	Ui::MainWindow *ui;

	QSerialPort _serialA, _serialB;
};

#endif // MAINWINDOW_H
