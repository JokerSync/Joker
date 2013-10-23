#include "MainView.h"
#include "ui_MainView.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>

#include "PhCommonUI/PhTimeCodeDialog.h"

MainView::MainView()
	: QMainWindow(0),
      ui(new Ui::MainView)//, _internalClock(PhTimeCodeType25)
{
	ui->setupUi(this);
    ui->mediaController->setClock(_videoEngine.clock());

	ui->_videoView->setEngine(&_videoEngine);
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

		_videoEngine.clock()->setFrame(0);
        _videoEngine.clock()->setRate(1.0);
		return true;
    }
	return false;
}

void MainView::on_actionPlay_pause_triggered()
{
    if(_videoEngine.clock()->rate()!=0)
        _videoEngine.clock()->setRate(0);
	else
        _videoEngine.clock()->setRate(1);
}

void MainView::on_actionNext_frame_triggered()
{
    _videoEngine.clock()->setFrame(_videoEngine.clock()->frame() + 1);
}

void MainView::on_actionPrevious_frame_triggered()
{
    _videoEngine.clock()->setFrame(_videoEngine.clock()->frame() - 1);
}

void MainView::on_actionSet_timestamp_triggered()
{
    PhTimeCodeDialog dlg(_videoEngine.clock()->timeCodeType(), _videoEngine.clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
	{
		PhFrame frameStamp = _videoEngine.frameStamp();
        frameStamp += dlg.frame() - _videoEngine.clock()->frame();
		_videoEngine.setFrameStamp(frameStamp);
		ui->mediaController->setFirstFrame(frameStamp);
        _videoEngine.clock()->setFrame(dlg.frame());
	}
}

void MainView::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    openFile(fileName); // TODO: show error in case of error
}

void MainView::on_actionReverse_triggered()
{
    _videoEngine.clock()->setRate(-1);
}
