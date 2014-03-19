#include "AVTestWindow.h"
#include "ui_AVTestWindow.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

#include "PhCommonUI/PhTimeCodeDialog.h"

AVTestWindow::AVTestWindow(QSettings *settings)
	: QMainWindow(0),
	ui(new Ui::AVTestWindow),
	_settings(settings),
	_videoEngine(true)
{
	ui->setupUi(this);
	_videoEngine.setSettings(settings);
	_mediaPanelDialog.setClock(_videoEngine.clock());
	_mediaPanelDialog.show();

	ui->_videoView->setEngine(&_videoEngine);
}

AVTestWindow::~AVTestWindow()
{
	delete ui;
}

bool AVTestWindow::openFile(QString fileName)
{
	QFileInfo fileInfo(fileName);
	if (fileInfo.exists()) {
		if(_videoEngine.open(fileName)) {
			this->setWindowTitle(fileName);
			_mediaPanelDialog.setMediaLength(_videoEngine.length());

			PhFrame frameStamp = _videoEngine.frameStamp();
			_mediaPanelDialog.setFirstFrame(frameStamp);

			_videoEngine.clock()->setFrame(frameStamp);
			//_videoEngine.clock()->setRate(1.0);
			_settings->setValue("lastVideoFile", fileName);
			return true;
		}
	}
	return false;
}

void AVTestWindow::resizeEvent(QResizeEvent *)
{
	PHDEBUG << this->width() << this->height();
	_mediaPanelDialog.move(this->x() + this->width() / 2 - _mediaPanelDialog.width() / 2,
	                       this->y() + this->height() * 0.95 - _mediaPanelDialog.height());
}

void AVTestWindow::on_actionPlay_pause_triggered()
{
	if(_videoEngine.clock()->rate() != 0)
		_videoEngine.clock()->setRate(0);
	else
		_videoEngine.clock()->setRate(1);
}

void AVTestWindow::on_actionNext_frame_triggered()
{
	_videoEngine.clock()->setFrame(_videoEngine.clock()->frame() + 1);
}

void AVTestWindow::on_actionPrevious_frame_triggered()
{
	_videoEngine.clock()->setFrame(_videoEngine.clock()->frame() - 1);
}

void AVTestWindow::on_actionSet_timestamp_triggered()
{
	PhTimeCodeDialog dlg(_videoEngine.clock()->timeCodeType(), _videoEngine.clock()->frame());
	if(dlg.exec() == QDialog::Accepted) {
		PhFrame frameStamp = _videoEngine.frameStamp();
		frameStamp += dlg.frame() - _videoEngine.clock()->frame();
		_videoEngine.setFrameStamp(frameStamp);
		_mediaPanelDialog.setFirstFrame(frameStamp);
		_videoEngine.clock()->setFrame(dlg.frame());
	}
}

void AVTestWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"), QDir::homePath());
	if(!openFile(fileName))
		QMessageBox::critical(this, "Error", "Unable to open " + fileName);
}

void AVTestWindow::on_actionReverse_triggered()
{
	_videoEngine.clock()->setRate(-1);
}
