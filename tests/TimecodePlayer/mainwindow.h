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

	/**
	 * @brief changeValuePlayButton
	 * Change the state of _playButtonState when cliking on _playButton
	 */
	void pushPlayButton();

	/**
	 * @brief changeStateFastForwardButton
	 * Change the state of _fastForwardButtonState when clicking on _fastForwardButton
	 */
	void pushForwardButton();

	/**
	 * @brief changeStateFastRewindButton
	 * Change the state of _fastForwardButtonState when clicking on _fastForwardButton
	 */
	void pushRewindButton();

	void pushBackButton();

	void pushNextFrameButton();

	void pushPreviousFrameButton();

	void useSliderCursor(int position);

	void selectRate(int index);

	void backToBeginning();


private:

	Ui::MainWindow *ui;
	QTimer *_timer;
	PhClock *_clock;

};


#endif // MAINWINDOW_H
