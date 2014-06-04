#include "VideoStripTestWindow.h"
#include "ui_VideoStripTestWindow.h"

#include <QFileDialog>
#include <QFontDialog>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

VideoStripTestWindow::VideoStripTestWindow(VideoStripTestSettings *settings) :
	PhDocumentWindow(settings),
	ui(new Ui::VideoStripTestWindow),
	_settings(settings),
	_strip(settings),
	_doc(_strip.doc()),
	_videoEngine(settings)
{
	ui->setupUi(this);

	_synchronizer.setStripClock(_strip.clock());

	_synchronizer.setVideoClock(_videoEngine.clock());

	connect(ui->videoStripView, &PhGraphicView::beforePaint, _strip.clock(), &PhClock::tick);
	connect(ui->videoStripView, &PhGraphicView::paint, this, &VideoStripTestWindow::onPaint);
}

VideoStripTestWindow::~VideoStripTestWindow()
{
	delete ui;
}

bool VideoStripTestWindow::openDocument(QString fileName)
{
	PHDEBUG << "openFile : " << fileName;
	if(!_doc->openStripFile(fileName))
		return false;

	_strip.clock()->setTimeCodeType(_doc->timeCodeType());
	_strip.clock()->setTime(_doc->lastTime());
	this->setWindowTitle(fileName);

	QFileInfo fileInfo(_doc->videoFilePath());
	if (fileInfo.exists()) {
		_videoEngine.open(_doc->videoFilePath());
		_videoEngine.setFirstFrame(_doc->videoFrameIn());
	}

	setCurrentDocument(fileName);

	return true;
}

void VideoStripTestWindow::on_actionOpen_triggered()
{
	QFileDialog dlg(this, "Open...", "", "Rythmo files (*.detx *.joker)");
	if(dlg.exec()) {
		QString fileName = dlg.selectedFiles()[0];
		if(!openDocument(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void VideoStripTestWindow::on_actionPlay_pause_triggered()
{
	if(_strip.clock()->rate() == 0.0)
		_strip.clock()->setRate(1.0);

	else
		_strip.clock()->setRate(0.0);
}

void VideoStripTestWindow::on_actionPlay_backward_triggered()
{
	_strip.clock()->setRate(-1.0);
}

void VideoStripTestWindow::on_actionStep_forward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setFrame(_strip.clock()->frame() + 1);
}

void VideoStripTestWindow::on_actionStep_backward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setFrame(_strip.clock()->frame() - 1);
}

void VideoStripTestWindow::on_actionStep_time_forward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setTime(_strip.clock()->time() + 1);
}

void VideoStripTestWindow::on_actionStep_time_backward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setTime(_strip.clock()->time() - 1);
}

void VideoStripTestWindow::on_action_3_triggered()
{
	_strip.clock()->setRate(-3.0);
}

void VideoStripTestWindow::on_action_1_triggered()
{
	_strip.clock()->setRate(-1.0);
}

void VideoStripTestWindow::on_action_0_5_triggered()
{
	_strip.clock()->setRate(-0.5);
}

void VideoStripTestWindow::on_action0_triggered()
{
	_strip.clock()->setRate(0.0);
}

void VideoStripTestWindow::on_action0_5_triggered()
{
	_strip.clock()->setRate(0.5);
}

void VideoStripTestWindow::on_action1_triggered()
{
	_strip.clock()->setRate(1.0);
}

void VideoStripTestWindow::on_action3_triggered()
{
	_strip.clock()->setRate(3.0);
}

void VideoStripTestWindow::on_actionGo_To_triggered()
{
	PhTimeCodeDialog dlg(_strip.clock()->timeCodeType(), _strip.clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
		_strip.clock()->setFrame(dlg.frame());
}

void VideoStripTestWindow::on_actionOpen_Video_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"), QDir::homePath());
	openVideoFile(fileName); // TODO: show error in case of error
}


bool VideoStripTestWindow::openVideoFile(QString videoFileName)
{
	QFileInfo fileInfo(videoFileName);
	if (fileInfo.exists()) {
		_videoEngine.open(videoFileName);
		return true;
	}
	return false;
}

QMenu *VideoStripTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

void VideoStripTestWindow::on_actionSet_Time_Code_triggered()
{
	PhTimeCodeDialog dlg(_strip.clock()->timeCodeType(), _strip.clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
		_videoEngine.setFirstFrame(dlg.frame());

}

void VideoStripTestWindow::on_actionFull_screen_triggered()
{
	_fullScreenTimer.stop();
	this->setWindowState(Qt::WindowFullScreen);
}

void VideoStripTestWindow::on_actionTest_mode_triggered()
{
	_settings->setStripTestMode(!_settings->stripTestMode());
}

void VideoStripTestWindow::onPaint(int width, int height)
{
	int videoHeight = height * 3 / 4;
	int videoWidth = height;
	int videoX = (width - videoWidth) / 2;
	_strip.draw(0, videoHeight, width, height - videoHeight);
	_videoEngine.drawVideo(videoX, 0, videoWidth, videoHeight);
}
