#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QtWidgets>
#include <QString>
//#include <Qtimer>


namespace Ui {
class MainWindow;
}

typedef struct Timecode Timecode;
struct Timecode{
    int hours;
    int minutes;
    int seconds;
    int ms;
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
     * @brief _timecode
     */
    Timecode _timecodeValue;

};


#endif // MAINWINDOW_H
