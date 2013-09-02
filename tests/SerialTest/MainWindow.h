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
	void sendText1();
	void readText1();
	void sendText2();
	void readText2();

private:
	Ui::MainWindow *ui;

	QSerialPort _serial1, _serial2;
};

#endif // MAINWINDOW_H
