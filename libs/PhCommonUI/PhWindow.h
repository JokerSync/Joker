#ifndef PHWINDOW_H
#define PHWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>

#include "PhWindowSettings.h"

class PhWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit PhWindow(PhWindowSettings *settings, QWidget *parent = 0);

protected:
	void moveEvent(QMoveEvent *);
	void resizeEvent(QResizeEvent *);

private:
	PhWindowSettings *_settings;
};

#endif // PHWINDOW_H
