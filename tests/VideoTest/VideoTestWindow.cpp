#include "VideoTestWindow.h"
#include "ui_VideoTestWindow.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

#include "PhCommonUI/PhTimeCodeDialog.h"

VideoTestWindow::VideoTestWindow(VideoTestSettings *settings)
	: PhDocumentWindow(settings),
	ui(new Ui::VideoTestWindow),
	_settings(settings)
{
	ui->setupUi(this);
	_videoEngine.setSettings(settings);
	ui->_videoView->setGraphicSettings(settings);
	_mediaPanelDialog.setClock(_videoEngine.clock());
	_mediaPanelDialog.show();

	ui->_videoView->setEngine(&_videoEngine);
}

VideoTestWindow::~VideoTestWindow()
{
	delete ui;
}

bool VideoTestWindow::openDocument(QString fileName)
{
	if(!_videoEngine.open(fileName))
		return false;

	_mediaPanelDialog.setMediaLength(_videoEngine.length());
	PhFrame frameStamp = _videoEngine.firstFrame();
	_mediaPanelDialog.setFirstFrame(frameStamp);

	_videoEngine.clock()->setFrame(frameStamp);

	setCurrentDocument(fileName);

	return true;
}

void VideoTestWindow::processArg(int argc, char *argv[])
{
	PhDocumentWindow::processArg(argc, argv);
	for(int i = 1; i < argc; i++) {
		PhFrame firstFrame = PhTimeCode::frameFromString(argv[i], _videoEngine.clock()->timeCodeType());
		if(firstFrame) {
			_videoEngine.setFirstFrame(firstFrame);
			_videoEngine.clock()->setFrame(firstFrame);
		}
	}
}

void VideoTestWindow::resizeEvent(QResizeEvent *)
{
	PHDEBUG << this->width() << this->height();
	_mediaPanelDialog.move(this->x() + this->width() / 2 - _mediaPanelDialog.width() / 2,
	                       this->y() + this->height() * 0.95 - _mediaPanelDialog.height());
}

void VideoTestWindow::closeEvent(QCloseEvent *)
{
	_mediaPanelDialog.close();
}

QMenu *VideoTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

void VideoTestWindow::onApplicationActivate()
{
	_mediaPanelDialog.show();
}

void VideoTestWindow::onApplicationDeactivate()
{
	_mediaPanelDialog.hide();
}

void VideoTestWindow::on_actionPlay_pause_triggered()
{
	if(_videoEngine.clock()->rate() != 0)
		_videoEngine.clock()->setRate(0);
	else
		_videoEngine.clock()->setRate(1);
}

void VideoTestWindow::on_actionNext_frame_triggered()
{
	_videoEngine.clock()->setFrame(_videoEngine.clock()->frame() + 1);
}

void VideoTestWindow::on_actionPrevious_frame_triggered()
{
	_videoEngine.clock()->setFrame(_videoEngine.clock()->frame() - 1);
}

void VideoTestWindow::on_actionSet_timestamp_triggered()
{
	_mediaPanelDialog.hide();
	PhFrame frame;
	if(_videoEngine.clock()->frame() < _videoEngine.firstFrame())
		frame = _videoEngine.firstFrame();
	else if(_videoEngine.clock()->frame() > _videoEngine.firstFrame() + _videoEngine.length())
		frame = _videoEngine.lastFrame();
	else
		frame = _videoEngine.clock()->frame();

	PhTimeCodeDialog dlg(_videoEngine.clock()->timeCodeType(), frame);
	if(dlg.exec() == QDialog::Accepted) {
		PhFrame frameStamp;
		if(_videoEngine.clock()->frame() > _videoEngine.firstFrame() + _videoEngine.length())
			frameStamp = dlg.frame() - (_videoEngine.length() - 1);
		else if (_videoEngine.clock()->frame() < _videoEngine.firstFrame())
			frameStamp =  dlg.frame();
		else
			frameStamp = _videoEngine.firstFrame() + dlg.frame() - _videoEngine.clock()->frame();

		_videoEngine.setFirstFrame(frameStamp);
		_mediaPanelDialog.setFirstFrame(frameStamp);
		_videoEngine.clock()->setFrame(dlg.frame());
	}
	_mediaPanelDialog.show();
}

void VideoTestWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"), QDir::homePath());
	if(QFile::exists(fileName)) {
		if(!openDocument(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void VideoTestWindow::on_actionReverse_triggered()
{
	_videoEngine.clock()->setRate(-1);
}

void VideoTestWindow::on_actionGo_to_triggered()
{
	PhTimeCodeDialog dlg(_videoEngine.clock()->timeCodeType(), _videoEngine.clock()->frame(), this);

	if(dlg.exec() == QDialog::Accepted) {
		PHDEBUG << PhTimeCode::stringFromFrame(dlg.frame(), _videoEngine.clock()->timeCodeType());
		_videoEngine.clock()->setFrame(dlg.frame());
	}
	else
		PHDEBUG << "cancel";
}
