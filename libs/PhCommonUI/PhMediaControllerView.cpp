#include "PhMediaControllerView.h"
#include "ui_PhMediaControllerView.h"

PhMediaControllerView::PhMediaControllerView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PhMediaControllerView)
{
	ui->setupUi(this);


	//Buttons Init

	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	//connect(ui->_playButton, SIGNAL(clicked()), this, SLOT(ui->pushPlayButton()));

	ui->_fastForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
	//connect(ui->_fastForwardButton, SIGNAL(clicked()), this, SLOT(ui->pushForwardButton()));

	ui->_fastRewindButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
	//connect(ui->_fastRewindButton, SIGNAL(clicked()), this, SLOT(ui->pushRewindButton()));

	ui->_backButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
	//connect(ui->_backButton, SIGNAL(clicked()), this, SLOT(ui->pushBackButton()));

	//Label Init

	ui->_rateLabel->setStyleSheet("font:24pt");

	//ui->_rateLabel->setText("rate: "+QString::number(_clock->getRate()));
	ui->_rateLabel->setText("rate: 0");

	//ui->_timecodeLabel->setText(PhTimeCode::stringFromFrame(_clock->getFrame(),PhTimeCodeType25));
	ui->_timecodeLabel->setText("00:00:00:00");
	ui->_timecodeLabel->setStyleSheet("font:50pt");



	//Combobox Init

	ui->_rateSelectionBox->addItem("rate: 0");
	ui->_rateSelectionBox->addItem("rate: 1");
	ui->_rateSelectionBox->addItem("rate: 4");
	ui->_rateSelectionBox->addItem("rate:-4");
	//connect(ui->_rateSelectionBox, SIGNAL(activated(int)), this, SLOT(selectRate()));


	//Layout Init


	ui->_hLayoutTop->addWidget(ui->_timecodeLabel,2);
	ui->_hLayoutTop->addWidget(ui->_rateLabel);

	ui->_hLayoutBottom->addWidget(ui->_backButton);
	ui->_hLayoutBottom->addWidget(ui->_fastRewindButton);
	ui->_hLayoutBottom->addWidget(ui->_playButton);
	ui->_hLayoutBottom->addWidget(ui->_fastForwardButton);
	ui->_hLayoutBottom->addWidget(ui->_rateSelectionBox);

	//this->setLayout(ui->_vLayout);
	this->resize(600, 200);

	//Connections SIGNALS/SLOTS
	//connect(_clock, SIGNAL(rateChanged()), this, SLOT(updateRateLabel()));
	//connect(_clock, SIGNAL(frameChanged()), this, SLOT(updateFrameLabel()));
}


//void PhMediaControllerView::pushPlayButton()
//{
//	if(MainView::get_clock()->getRate() == 0)//If state = pause
//	{
//		_clock->setRate(1);
//		_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

//	}
//	else //If state = play
//	{
//		_clock->setRate(0);
//		_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
//	}
//}


//void PhMediaControllerView::updateFrame()
//{
//	_clock->tick();
//}


//void PhMediaControllerView::pushForwardButton()
//{
//	_clock->setRate(4);
//	_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
//}


//void PhMediaControllerView::pushRewindButton()
//{
//	_clock->setRate(-4);
//	_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
//}

//void PhMediaControllerView::pushBackButton()
//{
//	_clock->setRate(0);
//	_clock->setFrame(0);
//}

//void PhMediaControllerView::updateRateLabel()
//{
//	_rateLabel->setText("rate: "+QString::number(_clock->getRate()));
//}


//void PhMediaControllerView::updateFrameLabel()
//{
//	_timecodeLabel->setText(PhTimeCode::stringFromFrame(_clock->getFrame(),PhTimeCodeType25));
//}


//void PhMediaControllerView::selectRate()
//{
//	_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

//	if(_rateSelectionBox->currentIndex() == 0)
//	{
//		_clock->setRate(0);
//		_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
//	}

//	if(_rateSelectionBox->currentIndex() == 1)
//	_clock->setRate(1);

//	if(_rateSelectionBox->currentIndex() == 2)
//	_clock->setRate(4);

//	if(_rateSelectionBox->currentIndex() == 3)
//	_clock->setRate(-4);

//}





PhMediaControllerView::~PhMediaControllerView()
{
	delete ui;
}
