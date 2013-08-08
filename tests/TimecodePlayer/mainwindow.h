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
    void changeValuePlayButton();
    /**
     * @brief increaseValueTimecode
     * Update the timecode every 40*rateValue ms when play button is enabled
     */
    void increaseValueTimecode();
    /**
     * @brief changeStateFastForwardButton
     * Change the state of _fastForwardButtonState when clicking on _fastForwardButton
     */
    void changeStateFastForwardButton();
    /**
     * @brief changeStateFastRewardButton
     * Change the state of _fastForwardButtonState when clicking on _fastForwardButton
     */
    void changeStateFastRewardButton();


private:

    //Layout
    QGridLayout *_gLayout;

    //Controller section
    /**
     * @brief _playButton
     * Activate the playing mode. Becomes a pause button when already playing
     */
    QPushButton *_playButton;

    /**
     * @brief _backButton
     * Back to the beginning
     */
    QPushButton *_backButton;

    /**
     * @brief _fastForwardButton
     * fast forward when activated
     */
    QPushButton *_fastForwardButton;

    /**
     * @brief _fastRewardButton
     * fast reward when activated
     */
    QPushButton *_fastRewardButton;

    /**
     * @brief _rateSelectionBox
     * Provides the selection of the rate
     */
    QComboBox *_rateSelectionBox;


    //View Section
    /**
     * @brief _timecode
     */
	QLabel *_timecodeLabel;

    /**
     * @brief _rate
     */
	QLabel *_rateLabel;


    //Model Section
    /**
     * @brief _heightWindow
     */
    int _heightWindow;

    /**
     * @brief _widthWindow
     */
    int _widthWindow;

	PhClock _clock;

};


#endif // MAINWINDOW_H
