#include "MainView.h"
#include "ui_MainView.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>

#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhVideoSynchronizer.h"

MainView::MainView()
	: QMainWindow(0),
      ui(new Ui::MainView), _internalClock(PhTimeCodeType25)
{
	ui->setupUi(this);
    ui->mediaController->setClock(&_internalClock);

	ui->_videoView->setEngine(&_videoEngine);

    _synchronizer.setVideoClock(_videoEngine.clock());
    _synchronizer.setInternalClock(&_internalClock);

    connect(&timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

	timer.start(40);
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
		if(!_videoEngine.open(fileName))
			return false;
#warning TODO read media length from video file
		ui->mediaController->setMediaLength(1000);
#warning TODO read first frame from video file
		ui->mediaController->setFirstFrame(0);

		_internalClock.setFrame(0);
        _internalClock.setRate(1.0);
		return true;
    }
	return false;
}

void MainView::onTimeOut()
{
    _internalClock.tick(25);
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
    PhTimeCodeDialog dlg(_internalClock.timeCodeType(), _internalClock.frame());
	if(dlg.exec() == QDialog::Accepted)
	{
		PhFrame frameStamp = _videoEngine.frameStamp();
        frameStamp += dlg.frame() - _internalClock.frame();
		_videoEngine.setFrameStamp(frameStamp);
		ui->mediaController->setFirstFrame(frameStamp);
        _internalClock.setFrame(dlg.frame());
	}
}

void MainView::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    openFile(fileName); // TODO: show error in case of error
}

void MainView::on_actionReverse_triggered()
{
    _internalClock.setRate(-1);
}
