#ifndef TIMECODEPLAYERWINDOW_H
#define TIMECODEPLAYERWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "PhCommonUI/PhMediaPanel.h"
#include "PhSync/PhClock.h"

namespace Ui {
class TimecodePlayerWindow;
}

class TimecodePlayerWindow : public QMainWindow
{
	Q_OBJECT

public:

	explicit TimecodePlayerWindow(QWidget *parent = 0);
	~TimecodePlayerWindow();

public slots:

	void updateFrame();

private:

	Ui::TimecodePlayerWindow *ui;
	QTimer *_timer;
	PhClock _clock;

};


#endif // TIMECODEPLAYERWINDOW_H
