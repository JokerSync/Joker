#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "PhCommonUI/PhMediaPanel.h"
#include "PhSync/PhClock.h"

namespace Ui {
class TimecodePlayerWindow;
}

class SonyToolWindow : public QMainWindow
{
	Q_OBJECT

public:

	explicit SonyToolWindow(QWidget *parent = 0);
	~SonyToolWindow();

public slots:

	void updateFrame();

private:

	Ui::TimecodePlayerWindow *ui;
	QTimer *_timer;
	PhClock _clock;

};


#endif // MAINWINDOW_H
