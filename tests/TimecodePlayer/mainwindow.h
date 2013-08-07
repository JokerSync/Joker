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
     * Provides the clock converstion (24h:60min:60sec:100cs)
     */
    void counter()
    {
        if(hundredth == 100)
        {
            hundredth = 0;
            seconds += 1;
        }
        if(seconds == 60)
        {
            seconds = 0;
            minutes += 1;
        }
        if(minutes == 60)
        {
            minutes = 0;
            hours += 1;
        }
        if(hours == 24)
            hours = 0;
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
    double get_rateValue() const;

    //Methods



    ~MainWindow();


public slots:
    /**
     * @brief changeValuePlayButton
     * Change the state of _playButtonState when cliking on _plauButton
     */
    void changeValuePlayButton();
    void increaseValueTimecode();


private:
    Ui::MainWindow *_ui;

    //Controller section
    /**
     * @brief _playButton
     */
    QPushButton *_playButton;// play/pause button
  /*  QPushButton *_stopButton;// stop button + back to the beginning
    QPushButton *_fastForwardButton;// fast forward when activated
    QPushButton *_fastRewardButton;// fast reward when activated
*/
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
     * true when playing, false when pause
     */
    bool _playButtonState;
    /**
     * @brief _rateValue
     * Value of the rate (0=pause, 1=play)
     */
    double _rateValue;
    /**
     * @brief _timer
     */
    QTimer *_timer;
    /**
     * @brief _timecode
     */
    Timecode _timecodeValue;

};


#endif // MAINWINDOW_H
