#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "PhCommonUI/PhMediaControllerView.h"


/**
 * @brief Timecode
 *
 */


class MainWindow : public QWidget
{
    Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = 0);

public slots:

	void updateFrame();

private:

	PhMediaControllerView *_mediaControllerView;
	PhClock *_clock;
	QTimer *_timer;
	PhFrame _fps;

};


#endif // MAINWINDOW_H
