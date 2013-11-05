#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QFontDialog>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFontDialog.h"
#include "AboutMenu.h"
#include "PreferencesDialog.h"
#include "PrefPanel.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_settings("Phonations", "Joker"),
	_sonySlave(PhTimeCodeType25, &_settings, this),
	#warning TODO check default speed
	_mediaPanelAnimation(&_mediaPanel, "windowOpacity")
{
	// Setting up UI
	ui->setupUi(this);

	// Get the pointer to the differents objects :
	// strip, video engine and doc
	_strip = ui->videoStripView->strip();
	_videoEngine = ui->videoStripView->videoEngine();
	_doc = _strip->doc();

	// Pass the settings to the modules
	_strip->setSettings(&_settings);
	_videoEngine->setSettings(&_settings);
	ui->videoStripView->setSettings(&_settings);

	// Initialize the synchronizer
	_synchronizer.setStripClock(_strip->clock());
	_synchronizer.setVideoClock(_videoEngine->clock());

	// Initialize the sony module
	if(_settings.value("sonyAutoConnect", true).toBool())
	{
		if(_sonySlave.open())
			_synchronizer.setSonyClock(_sonySlave.clock());
		else
			QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony slave");
	}

	// Setting up the media panel
	_mediaPanel.setClock(_strip->clock());
	_mediaPanel.show();
	_mediaPanelState = MediaPanelVisible;

	// Trigger a timer that will fade off the media panel after 3 seconds
	this->connect(&_mediaPanelTimer, SIGNAL(timeout()), this, SLOT(on_mediaPanelTimer_timeout()));
	_mediaPanelTimer.start(3000);

	// Set up a filter for catching mouse move event (see eventFilter())
	// that will show the media panel back
	qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openFile(QString fileName)
{
	PHDEBUG << "openFile : " << fileName;
	//PhString fileName = QFileDialog::getOpenFileName(this, tr("Open a script"),QDir::homePath(), "Script File (*.detx)");

	// Checking if the file exists
	if(QFile::exists(fileName))
	{
		// Try to open the document
		if(_doc->openDetX(fileName))
		{
			// On succeed, synchronizing the clocks
			_strip->clock()->setTimeCodeType(_doc->getTCType());
			_strip->clock()->setFrame(_doc->getLastFrame());
			this->setWindowTitle(fileName);

			// Opening the corresponding video file if it exists
			QFileInfo fileInfo(_doc->getVideoPath());
			if (fileInfo.exists())
			{
				_videoEngine->open(_doc->getVideoPath());
				_videoEngine->setFrameStamp(_doc->getVideoTimestamp());
				_mediaPanel.setFirstFrame(_doc->getVideoTimestamp());
				_mediaPanel.setMediaLength(_videoEngine->length());
			}
		}
	}
}

bool MainWindow::eventFilter(QObject *sender, QEvent *event)
{
	if(event->type() == QEvent::MouseMove)
	{
		_mediaPanel.show();
		_mediaPanelAnimation.stop();
		_mediaPanelAnimation.setDuration(300);
		_mediaPanelAnimation.setEndValue(1);
		_mediaPanelAnimation.setEasingCurve(QEasingCurve::InOutSine);
		_mediaPanelAnimation.start();
		_mediaPanelState = MediaPanelVisible;
		_mediaPanelTimer.start(3000);
	}
	return false;
}

void MainWindow::on_actionOpen_triggered()
{
	QFileDialog dlg(this, "Open...", "", "Rythmo files (*.detx)");
	if(dlg.exec())
	{
		QString fileName = dlg.selectedFiles()[0];
		openFile(fileName);
	}
}

void MainWindow::on_actionPlay_pause_triggered()
{
	if(_strip->clock()->rate() == 0.0)
		_strip->clock()->setRate(1.0);

	else
		_strip->clock()->setRate(0.0);
}

void MainWindow::on_actionPlay_backward_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void MainWindow::on_actionStep_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() + 1);
}

void MainWindow::on_actionStep_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() - 1);
}

void MainWindow::on_actionStep_time_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() + 1);
}

void MainWindow::on_actionStep_time_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() - 1);
}

void MainWindow::on_action_3_triggered()
{
	_strip->clock()->setRate(-3.0);
}

void MainWindow::on_action_1_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void MainWindow::on_action_0_5_triggered()
{
	_strip->clock()->setRate(-0.5);
}

void MainWindow::on_action0_triggered()
{
	_strip->clock()->setRate(0.0);
}

void MainWindow::on_action0_5_triggered()
{
	_strip->clock()->setRate(0.5);
}

void MainWindow::on_action1_triggered()
{
	_strip->clock()->setRate(1.0);
}

void MainWindow::on_action3_triggered()
{
	_strip->clock()->setRate(3.0);
}

void MainWindow::on_actionGo_To_triggered()
{
	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), _strip->clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
		_strip->clock()->setFrame(dlg.frame());
}

void MainWindow::on_actionOpen_Video_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
	if(openVideoFile(fileName))
		on_actionChange_timestamp_triggered();
	else
		QMessageBox::critical(this, "Error", "Unable to open " + fileName);
}

bool MainWindow::openVideoFile(QString videoFileName)
{
	QFileInfo fileInfo(videoFileName);
	if (fileInfo.exists())
	{
		_videoEngine->open(videoFileName);
		//#warning TODO read media length from video file
		//		ui->mediaController->setMediaLength(7500);
		//#warning TODO read first frame from video file
		//		ui->mediaController->setFirstFrame(0);

		//_clock->setRate(0.0);
		return true;
	}
	return false;
}

void MainWindow::on_actionChange_font_triggered()
{
	QString fontFile = QFileDialog::getOpenFileName(this, "Change font...", "", "Font files (*.ttf)");
	if(QFile(fontFile).exists())
	{
		if(!_strip->setFontFile(fontFile))
			QMessageBox::critical(this, "Error", "Unable to open " + fontFile);
	}
}

void MainWindow::on_actionChange_timestamp_triggered()
{
	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), _synchronizer.videoClock()->frame());
	if(dlg.exec() == QDialog::Accepted)
	{
		PhFrame frameStamp = _videoEngine->frameStamp();
		frameStamp += dlg.frame() - _synchronizer.videoClock()->frame();
		_videoEngine->setFrameStamp(frameStamp);
		_strip->clock()->setFrame(dlg.frame());
	}
}



void MainWindow::on_actionAbout_triggered()
{
	AboutMenu menu;
	menu.exec();
}


void MainWindow::on_actionPreferences_triggered()
{
//	PreferencesDialog dlg(&_settings);
//	dlg.exec();
	PrefPanel dlg(&_settings);
	dlg.exec();
}

void MainWindow::on_mediaPanelTimer_timeout()
{
	PHDEBUG << _mediaPanelState;
	switch(_mediaPanelState)
	{
	case MediaPanelVisible:
		_mediaPanelAnimation.setDuration(1000);
		_mediaPanelAnimation.setEndValue(0);
		_mediaPanelAnimation.setEasingCurve(QEasingCurve::InOutSine);
		_mediaPanelAnimation.start();
		_mediaPanelTimer.start(1000);
		_mediaPanelState = MediaPanelHidding;
		break;
	case MediaPanelHidding:
		_mediaPanel.hide();
		_mediaPanelState = MediaPanelHidden;
		_mediaPanelTimer.stop();
		break;
	}
}

