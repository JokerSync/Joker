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
	_settings(settings),
	_videoEngine(settings),
	_maxVideoRate(0)
{
	ui->setupUi(this);
	ui->videoView->setGraphicSettings(settings);

	_mediaPanelDialog.setClock(_videoEngine.timeCodeType(), _videoEngine.clock());

	connect(&_videoEngine, &PhVideoEngine::timeCodeTypeChanged, &_mediaPanelDialog, &PhMediaPanel::onTimeCodeTypeChanged);

	ui->actionDisplay_media_panel->setChecked(_settings->displayMediaPanel());
	ui->actionDeinterlace_video->setChecked(_settings->deinterlaceVideo());

	_videoEngine.setDeinterlace(_settings->deinterlaceVideo());

	connect(ui->videoView, &PhGraphicView::paint, this, &VideoTestWindow::onPaint);
	connect(ui->videoView, &PhGraphicView::beforePaint, _videoEngine.clock(), &PhClock::tick);
	connect(_videoEngine.clock(), SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
}

VideoTestWindow::~VideoTestWindow()
{
	delete ui;
}

bool VideoTestWindow::openDocument(QString fileName)
{
	if(!_videoEngine.open(fileName))
		return false;

	PhTimeCodeType tcType = _videoEngine.timeCodeType();
	_mediaPanelDialog.setLength(_videoEngine.length() * PhTimeCode::timePerFrame(tcType));
	PhFrame frameStamp = _videoEngine.firstFrame();
	PhFrame currentFrame = frameStamp;

	if(fileName == _settings->currentDocument()) {
		frameStamp = _settings->frameStamp();
		_videoEngine.setFirstFrame(frameStamp);
		currentFrame = _settings->currentFrame();
	}
	else if(_videoEngine.firstFrame() == 0) {
		on_actionSet_timestamp_triggered();
		frameStamp = _videoEngine.firstFrame();
		currentFrame = frameStamp;
	}
	_mediaPanelDialog.setTimeIn(frameStamp * PhTimeCode::timePerFrame(tcType));

	_videoEngine.clock()->setFrame(currentFrame, tcType);

	setCurrentDocument(fileName);
	_settings->setFrameStamp(frameStamp);

	return true;
}

void VideoTestWindow::processArg(int argc, char *argv[])
{
	PhDocumentWindow::processArg(argc, argv);
	for(int i = 1; i < argc; i++) {
		PhFrame firstFrame = PhTimeCode::frameFromString(argv[i], _videoEngine.timeCodeType());
		if(firstFrame) {
			_videoEngine.setFirstFrame(firstFrame);
			_videoEngine.clock()->setFrame(firstFrame, _videoEngine.timeCodeType());
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
	if(_settings->displayMediaPanel())
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
	_videoEngine.clock()->setTime(_videoEngine.clock()->time() + PhTimeCode::timePerFrame(_videoEngine.timeCodeType()));
}

void VideoTestWindow::on_actionPrevious_frame_triggered()
{
	_videoEngine.clock()->setTime(_videoEngine.clock()->time() - PhTimeCode::timePerFrame(_videoEngine.timeCodeType()));
}

void VideoTestWindow::on_actionSet_timestamp_triggered()
{
	_mediaPanelDialog.hide();
	PhTimeCodeType tcType = _videoEngine.timeCodeType();
	PhFrame frame;
	if(_videoEngine.clock()->frame(tcType) < _videoEngine.firstFrame())
		frame = _videoEngine.firstFrame();
	else if(_videoEngine.clock()->frame(tcType) > _videoEngine.firstFrame() + _videoEngine.length())
		frame = _videoEngine.lastFrame();
	else
		frame = _videoEngine.clock()->frame(tcType);

	PhTimeCodeDialog dlg(_videoEngine.timeCodeType(), frame * PhTimeCode::timePerFrame(tcType));
	if(dlg.exec() == QDialog::Accepted) {
		PhFrame frameStamp;
		if(_videoEngine.clock()->frame(tcType) > _videoEngine.firstFrame() + _videoEngine.length())
			frameStamp = dlg.frame() - (_videoEngine.length() - 1);
		else if (_videoEngine.clock()->frame(tcType) < _videoEngine.firstFrame())
			frameStamp =  dlg.frame();
		else
			frameStamp = _videoEngine.firstFrame() + dlg.frame() - _videoEngine.clock()->frame(tcType);

		_videoEngine.setFirstFrame(frameStamp);
		_mediaPanelDialog.setTimeIn(frameStamp * PhTimeCode::timePerFrame(tcType));
		_videoEngine.clock()->setFrame(dlg.frame(), tcType);
		_settings->setFrameStamp(frameStamp);
	}

	if(_settings->displayMediaPanel())
		_mediaPanelDialog.show();
}

void VideoTestWindow::on_actionOpen_triggered()
{
	_mediaPanelDialog.hide();
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"), _settings->lastDocumentFolder());
	if(QFile::exists(fileName)) {
		if(!openDocument(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}

	if(_settings->displayMediaPanel())
		_mediaPanelDialog.show();
}

void VideoTestWindow::on_actionReverse_triggered()
{
	_videoEngine.clock()->setRate(-1);
}

void VideoTestWindow::on_actionGo_to_triggered()
{
	_mediaPanelDialog.hide();
	PhTimeCodeDialog dlg(_videoEngine.timeCodeType(), _videoEngine.clock()->time(), this);

	if(dlg.exec() == QDialog::Accepted) {
		PHDEBUG << PhTimeCode::stringFromFrame(dlg.frame(), _videoEngine.timeCodeType());
		_videoEngine.clock()->setFrame(dlg.frame(), _videoEngine.timeCodeType());
	}

	if(_settings->displayMediaPanel())
		_mediaPanelDialog.show();
}

void VideoTestWindow::on_actionDisplay_media_panel_triggered(bool checked)
{
	_settings->setDisplayMediaPanel(checked);
	if(checked)
		_mediaPanelDialog.show();
	else
		_mediaPanelDialog.hide();
}

void VideoTestWindow::on_actionDeinterlace_video_triggered(bool checked)
{
	_settings->setDeinterlaceVideo(checked);
	_videoEngine.setDeinterlace(checked);
}

void VideoTestWindow::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	_settings->setCurrentFrame(frame);
	ui->statusBar->showMessage(PhTimeCode::stringFromFrame(frame, tcType));

}

void VideoTestWindow::onPaint(int width, int height)
{
	int videoRate = _videoEngine.refreshRate();
	if(videoRate > _maxVideoRate)
		_maxVideoRate = videoRate;
	QString info = QString("%1 / %2").arg(videoRate).arg(_maxVideoRate);
	ui->videoView->addInfo(info);
	_videoEngine.drawVideo(0, 0, width, height);
}
