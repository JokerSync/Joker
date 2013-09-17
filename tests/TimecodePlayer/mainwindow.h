#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "PhCommonUI/PhMediaPanel.h"
#include "PhTools/PhClock.h"

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
	QTimer *_timer;
	PhClock *_clock;

};


#endif // MAINWINDOW_H
