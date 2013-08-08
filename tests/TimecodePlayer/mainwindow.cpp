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
    _rateValue = 0;
    _timecodeValue = PhTimeCodeType25;

    //Buttons Init

    _playButton = new QPushButton("");

    _playButton->resize(50,50);
    _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    QObject::connect(_playButton, SIGNAL(clicked()), this, SLOT(changeValuePlayButton()));

    _fastForwardButton = new QPushButton("");
    _fastForwardButton->resize(50,50);
    _fastForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    QObject::connect(_fastForwardButton, SIGNAL(clicked()), this, SLOT(changeStateFastForwardButton()));

    _fastRewardButton = new QPushButton("");
    _fastRewardButton->resize(50,50);
    _fastRewardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    QObject::connect(_fastRewardButton, SIGNAL(clicked()), this, SLOT(changeStateFastRewardButton()));

    //Label Init

    _rate = new QLabel(QString::number(_rateValue), this);

    _timecode = new QLabel(PhTimeCode::stringFromFrame(_timecodeValue,PhTimeCodeType25), this);

    //Combobox Init
    _rateSelectionBox = new QComboBox;

    //Timer Init
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(increaseValueTimecode()));
    _timer->start(40);

    _gLayout = new QGridLayout;
    _gLayout->addWidget(_playButton,1,1);
    _gLayout->addWidget(_fastForwardButton,1,2);
    _gLayout->addWidget(_fastRewardButton,1,0);

    _gLayout->addWidget(_timecode,0,0);
    _gLayout->addWidget(_rate,0,1);


}


/****************************Slots****************************/


void MainWindow::changeValuePlayButton()
{
    if(_rateValue == 0)//If state = pause
    {
        _rateValue = 1;
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

    }
    else //If state = play
    {
        _rateValue = 0;
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
    _rate->setNum(_rateValue);
}


void MainWindow::increaseValueTimecode()
{
    /*_timecodeValue += 4*_rateValue;
    _timecodeValue.counter();
    _timecode->setText(_timecodeValue.displayFormat());*/
}


void MainWindow::changeStateFastForwardButton()
{
    _rateValue = 4;
    _rate->setNum(_rateValue);
}


void MainWindow::changeStateFastRewardButton()
{
    _rateValue = -4;
    _rate->setNum(_rateValue);
}

/****************************Methods****************************/

QGridLayout *MainWindow::getLayout() const
{
    return _gLayout;
}


MainWindow::~MainWindow()
{
    delete _timer;
    delete _timecode;
    delete _playButton;
    delete _fastForwardButton;
    delete _fastRewardButton;
    delete _rateSelectionBox;
    delete _rate;
    delete _gLayout;
    delete _ui;
}
