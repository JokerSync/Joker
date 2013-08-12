#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{

	_clock = new PhClock;
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(40);

    //Buttons Init

    _playButton = new QPushButton;
    _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	connect(_playButton, SIGNAL(clicked()), this, SLOT(pushPlayButton()));

    _fastForwardButton = new QPushButton;
    _fastForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
	connect(_fastForwardButton, SIGNAL(clicked()), this, SLOT(pushForwardButton()));

    _fastRewindButton = new QPushButton;
    _fastRewindButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
	connect(_fastRewindButton, SIGNAL(clicked()), this, SLOT(pushRewindButton()));

    _backButton = new QPushButton;
    _backButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
	connect(_backButton, SIGNAL(clicked()), this, SLOT(pushBackButton()));

    //Label Init

	_rateLabel = new QLabel;
	_rateLabel->setStyleSheet("font:24pt");
	_rateLabel->setText("rate: "+QString::number(_clock->getRate()));

	_timecodeLabel = new QLabel(PhTimeCode::stringFromFrame(_clock->getFrame(),PhTimeCodeType25));
	_timecodeLabel->setStyleSheet("font:50pt");



	//Combobox Init

    _rateSelectionBox = new QComboBox;
	_rateSelectionBox->addItem("rate: 0");
	_rateSelectionBox->addItem("rate: 1");
	_rateSelectionBox->addItem("rate: 4");
	_rateSelectionBox->addItem("rate:-4");
	connect(_rateSelectionBox, SIGNAL(activated(int)), this, SLOT(selectRate()));


	//Layout Init
	_hLayoutTop = new QHBoxLayout;
	_hLayoutBottom = new QHBoxLayout;
	_vLayout = new QVBoxLayout;

	_hLayoutTop->addWidget(_timecodeLabel,2);
	_hLayoutTop->addWidget(_rateLabel);

	_hLayoutBottom->addWidget(_backButton);
	_hLayoutBottom->addWidget(_fastRewindButton);
	_hLayoutBottom->addWidget(_playButton);
	_hLayoutBottom->addWidget(_fastForwardButton);
	_hLayoutBottom->addWidget(_rateSelectionBox);

	_vLayout->addLayout(_hLayoutTop);
	_vLayout->addLayout(_hLayoutBottom);

	this->setLayout(_vLayout);
    this->resize(600, 200);

	//Connections SIGNALS/SLOTS
	connect(_clock, SIGNAL(rateChanged()), this, SLOT(updateRateLabel()));
	connect(_clock, SIGNAL(frameChanged()), this, SLOT(updateFrameLabel()));
}


/****************************Slots****************************/


void MainWindow::pushPlayButton()
{
	if(_clock->getRate() == 0)//If state = pause
    {
		_clock->setRate(1);
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

    }
    else //If state = play
    {
		_clock->setRate(0);
        _playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}


void MainWindow::updateFrame()
{
	_clock->tick();
}


void MainWindow::pushForwardButton()
{
	_clock->setRate(4);
	_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
}


void MainWindow::pushRewindButton()
{
	_clock->setRate(-4);
	_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
}

void MainWindow::pushBackButton()
{
	_clock->setRate(0);
	_clock->setFrame(0);
}

void MainWindow::updateRateLabel()
{
	_rateLabel->setText("rate: "+QString::number(_clock->getRate()));
}


void MainWindow::updateFrameLabel()
{
	_timecodeLabel->setText(PhTimeCode::stringFromFrame(_clock->getFrame(),PhTimeCodeType25));
}


void MainWindow::selectRate()
{
	_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

	if(_rateSelectionBox->currentIndex() == 0)
	{
		_clock->setRate(0);
		_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	}

	if(_rateSelectionBox->currentIndex() == 1)
	_clock->setRate(1);

	if(_rateSelectionBox->currentIndex() == 2)
	_clock->setRate(4);

	if(_rateSelectionBox->currentIndex() == 3)
	_clock->setRate(-4);

}

/****************************Methods****************************/

MainWindow::~MainWindow()
{


    delete _playButton;
    delete _fastForwardButton;
    delete _fastRewindButton;
    delete _backButton;

    delete _rateSelectionBox;
	delete _rateLabel;
	delete _timecodeLabel;
	delete _hLayoutTop;
	delete _hLayoutBottom;
	delete _vLayout;


	delete _timer;
	delete _clock;
}
