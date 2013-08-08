#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QtWidgets>
#include <QString>
#include <Qtimer>
#include <QDebug>
#include <QGridLayout>
#include <QtGui>
#include "PhTools/PhTimeCode.h"
#include "PhTools/PhTime.h"
#include "PhTools/PhString.h"

using namespace std;

namespace Ui {
class MainWindow;
}

/**
 * @brief Timecode
 *
 */


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);

    //Accessors
    /**
     * @brief get_rateValue
     * @return _rateValue
     */
    //Methods
    QGridLayout* getLayout() const;



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
    Ui::MainWindow *_ui;

    //Layout
    QGridLayout *_gLayout;

    //Controller section
    /**
     * @brief _playButton
     * Activate the playing mode. Becomes a pause button when already playing
     */
    QPushButton *_playButton;

    //QPushButton *_stopButton;// stop button + back to the beginning

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
    QLabel *_timecode;

    /**
     * @brief _rate
     */
    QLabel *_rate;


    //Model Section
    /**
     * @brief _heightWindow
     */
    int _heightWindow;

    /**
     * @brief _widthWindow
     */
    int _widthWindow;

    /**
     * @brief _rateValue
     * Value of the rate (0=pause, 1=play, 4=fastforward, -4=fastreward)
     */
    double _rateValue;

    /**
     * @brief _timer
     */
    QTimer *_timer;

    /**
     * @brief _timecode
     * Standard timecode format "00:00:00:00"
     */
    PhFrame _timecodeValue;

};


#endif // MAINWINDOW_H
