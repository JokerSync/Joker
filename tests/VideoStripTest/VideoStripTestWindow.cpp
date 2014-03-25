#include "VideoStripTestWindow.h"
#include "ui_VideoStripTestWindow.h"

#include <QFileDialog>
#include <QFontDialog>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

VideoStripTestWindow::VideoStripTestWindow(VideoStripTestSettings *settings) :
	PhDocumentWindow(settings),
	ui(new Ui::VideoStripTestWindow),
	_settings(settings)
{
	ui->setupUi(this);
	_strip = ui->videoStripView->strip();
	_videoEngine = ui->videoStripView->videoEngine();

	_strip->setSettings(_settings);

	_doc = _strip->doc();

	_synchronizer.setStripClock(_strip->clock());

	_synchronizer.setVideoClock(_videoEngine->clock());

	if(_settings->fullScreen()) {
		this->connect(&_fullScreenTimer, SIGNAL(timeout()), this, SLOT(on_actionFull_screen_triggered()));
		_fullScreenTimer.start(1000);
	}
}

VideoStripTestWindow::~VideoStripTestWindow()
{
	_settings->setFullScreen(this->windowState() == Qt::WindowFullScreen);
	delete ui;
}

bool VideoStripTestWindow::openDocument(QString fileName)
{
	PHDEBUG << "openFile : " << fileName;
	if(!_doc->openStripFile(fileName))
		return false;

	_strip->clock()->setTimeCodeType(_doc->timeCodeType());
	_strip->clock()->setFrame(_doc->lastFrame());
	this->setWindowTitle(fileName);

	QFileInfo fileInfo(_doc->videoFilePath());
	if (fileInfo.exists()) {
		_videoEngine->open(_doc->videoFilePath());
		_videoEngine->setFirstFrame(_doc->videoFramestamp());
	}

	setCurrentDocument(fileName);

	return true;
}

void VideoStripTestWindow::on_actionOpen_triggered()
{
	QFileDialog dlg(this, "Open...", "", "Rythmo files (*.detx *.strip)");
	if(dlg.exec()) {
		QString fileName = dlg.selectedFiles()[0];
		if(!openDocument(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void VideoStripTestWindow::on_actionPlay_pause_triggered()
{
	if(_strip->clock()->rate() == 0.0)
		_strip->clock()->setRate(1.0);

	else
		_strip->clock()->setRate(0.0);
}

void VideoStripTestWindow::on_actionPlay_backward_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void VideoStripTestWindow::on_actionStep_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() + 1);
}

void VideoStripTestWindow::on_actionStep_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() - 1);
}

void VideoStripTestWindow::on_actionStep_time_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() + 1);
}

void VideoStripTestWindow::on_actionStep_time_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() - 1);
}

void VideoStripTestWindow::on_action_3_triggered()
{
	_strip->clock()->setRate(-3.0);
}

void VideoStripTestWindow::on_action_1_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void VideoStripTestWindow::on_action_0_5_triggered()
{
	_strip->clock()->setRate(-0.5);
}

void VideoStripTestWindow::on_action0_triggered()
{
	_strip->clock()->setRate(0.0);
}

void VideoStripTestWindow::on_action0_5_triggered()
{
	_strip->clock()->setRate(0.5);
}

void VideoStripTestWindow::on_action1_triggered()
{
	_strip->clock()->setRate(1.0);
}

void VideoStripTestWindow::on_action3_triggered()
{
	_strip->clock()->setRate(3.0);
}

void VideoStripTestWindow::on_actionGo_To_triggered()
{
	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), _strip->clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
		_strip->clock()->setFrame(dlg.frame());
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
		_videoEngine->open(videoFileName);
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
	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), _strip->clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
		_videoEngine->setFirstFrame(dlg.frame());

}

void VideoStripTestWindow::on_actionChange_font_triggered()
{
	QString fontFile = QFileDialog::getOpenFileName(this, "Change font...", "", "Font files (*.ttf)");
	if(QFile(fontFile).exists()) {
		if(!_strip->setFontFile(fontFile))
			QMessageBox::critical(this, "Error", "Unable to open " + fontFile);
	}
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
