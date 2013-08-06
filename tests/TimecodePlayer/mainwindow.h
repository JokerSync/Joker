#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QtWidgets>
#include <QString>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int x, int y, QWidget *parent = 0);
    ~MainWindow();

public slots:


private:
    Ui::MainWindow *_ui;
    //Buttons
    QPushButton *_playButton;// play/pause button
    QPushButton *_stopButton;// stop button + back to the beginning
    QPushButton *_fastForwardButton;// fast forward when activated
    QPushButton *_fastRewardButton;// fast reward when activated
    QLabel *_label;
    QLabel *_label2;
    int _valueSlider;
    bool _valueButton;
};


#endif // MAINWINDOW_H
