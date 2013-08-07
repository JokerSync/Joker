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
    _fastForwardButtonState = false;
    _rateValue = 0;
    _timecodeValue.hours = 0;
    _timecodeValue.minutes = 0;
    _timecodeValue.seconds = 0;
    _timecodeValue.hundredth = 0;

    //Buttons Init

    _playButton = new QPushButton("", this);
    _playButton->setGeometry(_widthWindow/2-30,0.75*_heightWindow-30,60,60);
    _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    QObject::connect(_playButton, SIGNAL(clicked()), this, SLOT(changeValuePlayButton()));

    _fastForwardButton = new QPushButton("", this);
    _fastForwardButton->setGeometry(_widthWindow/2+30,0.75*_heightWindow-20,40,40);
    _fastForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    QObject::connect(_fastForwardButton, SIGNAL(clicked()), this, SLOT(changeStateFastForwardButton()));

    _fastRewardButton = new QPushButton("", this);
    _fastRewardButton->setGeometry(_widthWindow/2-30-40,0.75*_heightWindow-20,40,40);
    _fastRewardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    QObject::connect(_fastRewardButton, SIGNAL(clicked()), this, SLOT(changeStateFastRewardButton()));

    //Label Init
    QString text = "rate :" + QString::number(_rateValue);
    _rate = new QLabel(text, this);
    _rate->setGeometry(50+20,_heightWindow/2-25,100,50);

    _timecode = new QLabel(_timecodeValue.displayFormat(), this);
    _timecode->setGeometry((1-1/5)*_widthWindow-100-20,_heightWindow/2-25,200,50);

    //Timer Init
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(increaseValueTimecode()));
    _timer->start(40);
}


/****************************Slots****************************/


void MainWindow::changeValuePlayButton()
{
    if(_playButtonState == false)//If state = pause
    {
        _rateValue = 1;
        _playButtonState = true;
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

    }
    else //If state = play
    {
        _rateValue = 0;
        _playButtonState = false;
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    _rate->setNum(_rateValue);
}


void MainWindow::increaseValueTimecode()
{
    {
        _timecodeValue.hundredth += 4*_rateValue;
        _timecodeValue.counter();
        _timecode->setText(_timecodeValue.displayFormat());
    }
}


void MainWindow::changeStateFastForwardButton()
{
    if(_fastForwardButtonState == false)
    {
        _rateValue = 4;
        _fastForwardButtonState = true;
    }
    else
    {
        if(_playButtonState)
            _rateValue = 1;
        else
            _rateValue = 0;
        _fastForwardButtonState = false;

    }
    _playButtonState = false;
    _rate->setNum(_rateValue);
}


void MainWindow::changeStateFastRewardButton()
{
    if(_fastRewardButtonState == false)
    {
        _rateValue = -4;
        _fastRewardButtonState = true;
    }
    else
    {
        if(_playButtonState)
            _rateValue = 1;
        else
            _rateValue = 0;
        _fastRewardButtonState = false;

    }
    _playButtonState = false;
    _rate->setNum(_rateValue);
}

/****************************Methods****************************/




MainWindow::~MainWindow()
{
    delete _timer;
    delete _timecode;
    delete _playButton;
    delete _fastForwardButton;
    delete _fastRewardButton;
    delete _rate;
    delete _ui;
}
