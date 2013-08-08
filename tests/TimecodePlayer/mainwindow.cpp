#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    _widthWindow = 600;
    _heightWindow = 200;
    resize(_widthWindow,_heightWindow);

    //Buttons Init

    _playButton = new QPushButton;
    //_playButton->resize(50,50);
    _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    QObject::connect(_playButton, SIGNAL(clicked()), this, SLOT(changeValuePlayButton()));

    _fastForwardButton = new QPushButton;
    //_fastForwardButton->resize(50,50);
    _fastForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
    QObject::connect(_fastForwardButton, SIGNAL(clicked()), this, SLOT(changeStateFastForwardButton()));

    _fastRewardButton = new QPushButton;
    //_fastRewardButton->resize(50,50);
    _fastRewardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    QObject::connect(_fastRewardButton, SIGNAL(clicked()), this, SLOT(changeStateFastRewardButton()));

    _backButton = new QPushButton;
    _backButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    //Label Init
    QSizePolicy size_policy;
    size_policy.setVerticalStretch(20);
    size_policy.setHorizontalStretch(10);
	_rateLabel = new QLabel;
	_rateLabel->setSizePolicy(size_policy);
	_rateLabel->setText("rate: "+QString::number(_clock.get_rate()));

	_timecodeLabel = new QLabel(PhTimeCode::stringFromFrame(_clock.get_timecode(),PhTimeCodeType25));

    //Combobox Init
    _rateSelectionBox = new QComboBox;
    _gLayout = new QGridLayout;
    _gLayout->addWidget(_backButton,1,0);
    _gLayout->addWidget(_fastRewardButton,1,1);
    _gLayout->addWidget(_playButton,1,2);
    _gLayout->addWidget(_fastForwardButton,1,3);



	_gLayout->addWidget(_timecodeLabel,0,0);
	_gLayout->addWidget(_rateLabel,0,1);

    this->setLayout(_gLayout);


    this->resize(600, 200);
}


/****************************Slots****************************/


void MainWindow::changeValuePlayButton()
{
	if(_clock.get_rate() == 0)//If state = pause
    {
        _rateValue = 1;
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

    }
    else //If state = play
    {
        _rateValue = 0;
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
	_rateLabel->setText("rate: "+QString::number(_rateValue));
}


void MainWindow::increaseValueTimecode()
{
    _timecodeValue += _rateValue;
	_timecodeLabel->setText(PhTimeCode::stringFromFrame(_timecodeValue,PhTimeCodeType25));
}


void MainWindow::changeStateFastForwardButton()
{
    _rateValue = 4;
	_rateLabel->setText("rate: "+QString::number(_rateValue));
}


void MainWindow::changeStateFastRewardButton()
{
    _rateValue = -4;
	_rateLabel->setText("rate: "+QString::number(_rateValue));
}

/****************************Methods****************************/

MainWindow::~MainWindow()
{

    delete _timer;
	delete _timecodeLabel;

    delete _playButton;
    delete _fastForwardButton;
    delete _fastRewardButton;
    delete _backButton;

    delete _rateSelectionBox;
	delete _rateLabel;
    delete _gLayout;
}
