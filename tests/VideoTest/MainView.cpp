#include "MainView.h"
#include "ui_MainView.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

#include "PhCommonUI/PhTimeCodeDialog.h"

MainView::MainView(QSettings *settings)
	: QMainWindow(0),
      ui(new Ui::MainView),
	  _settings(settings)
{
	ui->setupUi(this);
	_mediaPanelDialog.setClock(_videoEngine.clock());
	_mediaPanelDialog.show();

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
		if(_videoEngine.open(fileName))
		{
			this->setWindowTitle(fileName);
#warning TODO read media length from video file
			_mediaPanelDialog.setMediaLength(1000);
#warning TODO read first frame from video file
			_mediaPanelDialog.setFirstFrame(0);

			_videoEngine.clock()->setFrame(0);
			_videoEngine.clock()->setRate(1.0);
			_settings->setValue("lastVideoFile", fileName);
			return true;
		}
    }
	return false;
}

void MainView::resizeEvent(QResizeEvent *)
{
	PHDEBUG << this->width() << this->height();
	_mediaPanelDialog.move(this->x() + this->width() / 2 - _mediaPanelDialog.width() / 2,
						   this->y() + this->height() * 0.95 - _mediaPanelDialog.height());
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
		_mediaPanelDialog.setFirstFrame(frameStamp);
        _videoEngine.clock()->setFrame(dlg.frame());
	}
}

void MainView::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
    if(!openFile(fileName))
		QMessageBox::critical(this, "Error", "Unable to open " + fileName);
}

void MainView::on_actionReverse_triggered()
{
    _videoEngine.clock()->setRate(-1);
}
