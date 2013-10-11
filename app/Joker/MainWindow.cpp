#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QFontDialog>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFontDialog.h"
#include "AboutMenu.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_settings("Phonations", "Joker"),
	_sonySlave(PhTimeCodeType25, &_settings, this)
#warning TODO check default speed
{
	ui->setupUi(this);
	_stripView = ui->stripView;

	if(!_stripView->setFont(_settings.value("StripFontName", "Arial").toString()))
		PHDEBUG << "The font has not been initialized";

	_doc = _stripView->doc();

	_stripClock = _stripView->clock();

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));

	_synchronizer.setStripClock(_stripClock);

	_synchronizer.setVideoClock(ui->videoView->getClock());

	if(_sonySlave.open())
		_synchronizer.setSonyClock(_sonySlave.clock());
	else
		QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony slave");
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openFile(QString fileName)
{
	PHDEBUG << "openFile : " << fileName;
  //  PhString fileName = QFileDialog::getOpenFileName(this, tr("Open a script"),QDir::homePath(), "Script File (*.detx)");
	if(QFile::exists(fileName))
	{
		if(_doc->openDetX(fileName))
		{
			_stripClock->setTimeCodeType(_doc->getTCType());
			_stripClock->setFrame(_doc->getLastFrame());
			this->setWindowTitle(fileName);

			QFileInfo fileInfo(_doc->getVideoPath());
			if (fileInfo.exists())
			{
				ui->videoView->open(_doc->getVideoPath());
				ui->videoView->setFrameStamp(_doc->getVideoTimestamp());
			}
		}
	}
}

void MainWindow::onOpenFile()
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
	if(_stripClock->rate() == 0.0)
		_stripClock->setRate(1.0);

	else
		_stripClock->setRate(0.0);
}

void MainWindow::on_actionPlay_backward_triggered()
{
	_stripClock->setRate(-1.0);
}

void MainWindow::on_actionStep_forward_triggered()
{
	_stripClock->setRate(0.0);
	_stripClock->setFrame(_stripClock->frame() + 1);
}

void MainWindow::on_actionStep_backward_triggered()
{
	_stripClock->setRate(0.0);
	_stripClock->setFrame(_stripClock->frame() - 1);
}

void MainWindow::on_actionStep_time_forward_triggered()
{
	_stripClock->setRate(0.0);
	_stripClock->setTime(_stripClock->time() + 1);
}

void MainWindow::on_actionStep_time_backward_triggered()
{
	_stripClock->setRate(0.0);
	_stripClock->setTime(_stripClock->time() - 1);
}

void MainWindow::on_action_3_triggered()
{
	_stripClock->setRate(-3.0);
}

void MainWindow::on_action_1_triggered()
{
	_stripClock->setRate(-1.0);
}

void MainWindow::on_action_0_5_triggered()
{
	_stripClock->setRate(-0.5);
}

void MainWindow::on_action0_triggered()
{
	_stripClock->setRate(0.0);
}

void MainWindow::on_action0_5_triggered()
{
	_stripClock->setRate(0.5);
}

void MainWindow::on_action1_triggered()
{
	_stripClock->setRate(1.0);
}

void MainWindow::on_action3_triggered()
{
	_stripClock->setRate(3.0);
}

void MainWindow::on_actionGo_To_triggered()
{
	PhTimeCodeDialog dlg(_stripClock->timeCodeType(), _stripClock->frame());
	if(dlg.exec() == QDialog::Accepted)
		_stripClock->setFrame(dlg.frame());
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
		ui->videoView->open(videoFileName);
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
	PhFontDialog fontBox;
	if(fontBox.exec())
	{
		QString font = fontBox.getFontSelected();
		if(_stripView->setFont(font))
			_settings.setValue("StripFontName", font);
		else
			QMessageBox::critical(this, "Error", "Unable to open " + font);
	}
}

void MainWindow::on_actionChange_timestamp_triggered()
{
	PhTimeCodeDialog dlg(_stripClock->timeCodeType(), _synchronizer.videoClock()->frame());
	if(dlg.exec() == QDialog::Accepted)
	{
		PhFrame frameStamp = ui->videoView->frameStamp();
	    frameStamp += dlg.frame() - _synchronizer.videoClock()->frame();
		ui->videoView->setFrameStamp(frameStamp);
	    _stripClock->setFrame(dlg.frame());
	}
}



void MainWindow::on_actionAbout_triggered()
{
	AboutMenu menu;
	menu.exec();
}

