#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _widthWindow = 600;
    _heightWindow = 200;
    resize(_widthWindow,_heightWindow);

    //Model Init
    _playButtonState = false;
    _rateValue = 0;
    _timecodeValue.hours = 0;
    _timecodeValue.minutes = 0;
    _timecodeValue.seconds = 0;
    _timecodeValue.ms = 0;

    //Buttons Init

    _playButton = new QPushButton("", this);
    _playButton->setGeometry(_widthWindow/2-30,0.75*_heightWindow-30,60,60);
    _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    QObject::connect(_playButton, SIGNAL(clicked()), this, SLOT(changeValuePlayButton()));

    //Label Init

    _rate = new QLabel(QString::number(_rateValue), this);
    _rate->setGeometry(50+20,_heightWindow/2-25,100,50);

    _timecode = new QLabel("00:00:00:00", this);
    _timecode->setGeometry((1-1/5)*_widthWindow-100-20,_heightWindow/2-25,200,50);



}


/****************************Slots****************************/


void MainWindow::changeValuePlayButton()
{
    if(_playButtonState == false)//If pause
    {
        _rateValue = 1;
        _playButtonState = true;
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

    }
    else //If playing
    {
        _rateValue = 0;
        _playButtonState = false;
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    _rate->setNum(_rateValue);
}

/****************************Accessors****************************/

double MainWindow::get_rateValue() const
{
    return _rateValue;
}


/****************************Methods****************************/




MainWindow::~MainWindow()
{
    delete _timecode;
    delete _playButton;
    delete _rate;
    delete _ui;
}
