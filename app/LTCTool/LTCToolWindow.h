#ifndef LTCTOOLWINDOW_H
#define LTCTOOLWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "PhTools/PhClock.h"
#include "PhSync/PhLtcWriter.h"

namespace Ui {
class LTCToolWindow;
}

class LTCToolWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit LTCToolWindow(QWidget *parent = 0);
	~LTCToolWindow();

public slots:

	void updateFrame();

private:
	Ui::LTCToolWindow *ui;
	PhClock *_clock;
	QTimer *_timer;

	PhLtcWriter _LTCWriter;



};

#endif // LTCTOOLWINDOW_H
