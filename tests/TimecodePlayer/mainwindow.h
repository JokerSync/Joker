#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "PhCommonUI/PhMediaControllerView.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:

	void updateFrame();

private:

	Ui::MainWindow *ui;
	PhClock *_clock;
	QTimer *_timer;

};


#endif // MAINWINDOW_H
