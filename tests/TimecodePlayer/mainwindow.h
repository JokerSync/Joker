#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QtWidgets>
#include <QString>
#include <Qtimer>
#include <QDebug>


namespace Ui {
class MainWindow;
}

/**
 * @brief Timecode
 *
 */
typedef struct Timecode Timecode;
struct Timecode{
    int hours;
    int minutes;
    int seconds;
    int hundredth;

    /**
     * @brief counter
     * Provides the clock conversion (24h:60min:60sec:100cs)
     */
    void counter()
    {
        if(hundredth >= 100)
        {
            hundredth = 0;
            seconds += 1;
        }
        if(hundredth < 0)
        {
            hundredth = 99;
            seconds -= 1;
        }
        if(seconds >= 60)
        {
            seconds = 0;
            minutes += 1;
        }
        if(seconds < 0)
        {
            seconds = 59;
            minutes -= 1;
        }
        if(minutes >= 60)
        {
            minutes = 0;
            hours += 1;
        }
        if(minutes < 0)
        {
            minutes = 59;
            hours -= 1;
        }
        if(hours >= 24)
            hours = 0;
        if(hours < 0)
            hours = 23;
    }

    /**
     * @brief displayFormat
     * @return QString with "00:00:00:00" format
     */
    QString displayFormat()
    {
        QString time;

        if(hours < 10)
            time = "0"+QString::number(hours)+":";
        else
            time = QString::number(hours)+":";

        if(minutes < 10)
            time = time+"0"+QString::number(minutes)+":";
        else
            time = time+QString::number(minutes)+":";

        if(seconds < 10)
            time = time+"0"+QString::number(seconds)+":";
        else
            time = time+QString::number(seconds)+":";

        if(hundredth < 10)
            time = time+"0"+QString::number(hundredth);
        else
            time = time+QString::number(hundredth);

        return time;
    }
};

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

    //Controller section
    /**
     * @brief _playButton
     * Activate the playing mode. Becomes a pause button when already playing
     */
    QPushButton *_playButton;// play/pause button
    //QPushButton *_stopButton;// stop button + back to the beginning
    /**
     * @brief _fastForwardButton
     */
    QPushButton *_fastForwardButton;// fast forward when activated
    /**
     * @brief _fastRewardButton
     */
    QPushButton *_fastRewardButton;// fast reward when activated

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
     * @brief _playButtonState
     * True when playing, false when pause
     */
    bool _playButtonState;
    /**
     * @brief _fastForwardButtonState
     * True when fastforwarding, false otherwise
     */
    bool _fastForwardButtonState;
    /**
     * @brief fastRewardButtonState
     * True when rewarding, false otherwise
     */
    bool _fastRewardButtonState;
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
    Timecode _timecodeValue;

};


#endif // MAINWINDOW_H
