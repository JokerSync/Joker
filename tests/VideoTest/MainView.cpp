#include "MainView.h"
#include "ui_MainView.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>

#include "PhCommonUI/PhTimeCodeDlg.h"
#include "PhVideoSynchronizer.h"

MainView::MainView()
	: QMainWindow(0),
      ui(new Ui::MainView), _internalClock(PhTimeCodeType25)
{   
	ui->setupUi(this);
    ui->mediaController->setClock(&_internalClock);

    _VideoSynchronizer.setVideoClock(ui->_videoView->getClock());
    _VideoSynchronizer.setInternalClock(&_internalClock);

    connect(&timer, SIGNAL(timeout()), this, SLOT(_VideoSunchronizer.onInternalTimeChanged()));

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));
	connect(&timer, SIGNAL(timeout()), ui->_videoView, SLOT(checkVideoPosition()));

	timer.start(10);
}

MainView::~MainView()
{
	delete ui;
}

bool MainView::openFile(QString fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
		ui->_videoView->open(fileName);
#warning TODO read media length from video file
		ui->mediaController->setMediaLength(7500);
#warning TODO read first frame from video file
		ui->mediaController->setFirstFrame(0);

        _internalClock.setRate(0.0);
		return true;
    }
	return false;
}


void MainView::onOpenFile()
{
	 QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
	 openFile(fileName); // TODO: show error in case of error
}

void MainView::onTimeOut()
{
    _internalClock.tick(100);
}



void MainView::on_actionPlay_pause_triggered()
{
    if(_internalClock.rate()!=0)
        _internalClock.setRate(0);
	else
        _internalClock.setRate(1);
}

void MainView::on_actionNext_frame_triggered()
{
    _internalClock.setFrame(_internalClock.frame() + 1);
}

void MainView::on_actionPrevious_frame_triggered()
{
    _internalClock.setFrame(_internalClock.frame() - 1);
}

void MainView::on_actionSet_timestamp_triggered()
{
    PhTimeCodeDlg dlg(_internalClock.timeCodeType(), _internalClock.frame());
	if(dlg.exec() == QDialog::Accepted)
	{
		PhFrame frameStamp = ui->_videoView->getFrameStamp();
        frameStamp += dlg.frame() - _internalClock.frame();
		ui->_videoView->setFrameStamp(frameStamp);
		ui->mediaController->setFirstFrame(frameStamp);
        _internalClock.setFrame(dlg.frame());
	}
}
