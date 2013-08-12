#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "PhTools/PhClock.h"


/**
 * @brief Timecode
 *
 */


class MainWindow : public QWidget
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);


    ~MainWindow();


public slots:

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

	/**
	 * @brief pushBacButton
	 */
	void pushBackButton();

	/**
	 * @brief selectRate
	 */
	void selectRate();



	/**
	 * @brief increaseValueTimecode
	 * Update the timecode every 40*rateValue ms when play button is enabled
	 */
	void updateFrame();


	/**
	 * @brief updateRateDisplay
	 */
	void updateRateLabel();

	/**
	 * @brief updateFrameLabel
	 */
	void updateFrameLabel();


private:

    //Layout
	QHBoxLayout *_hLayoutTop;
	QHBoxLayout *_hLayoutBottom;
	QVBoxLayout *_vLayout;

    //Controller section

    QPushButton *_playButton;
    QPushButton *_backButton;
    QPushButton *_fastForwardButton;
	QPushButton *_fastRewindButton;
    QComboBox *_rateSelectionBox;


	//View Section
	QLabel *_timecodeLabel;
	QLabel *_rateLabel;

	//Data
	QTimer *_timer;
	PhClock *_clock;

};


#endif // MAINWINDOW_H
