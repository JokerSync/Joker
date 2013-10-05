#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_stripView = ui->stripView;
	_doc = _stripView->doc();

	_stripClock = _stripView->clock();

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));

	connect(_stripClock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_stripClock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));

	_stripVideoSynchronizer.setStripClock(_stripClock);

	_stripVideoSynchronizer.setVideoClock(ui->videoView->getClock());
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openFile(QString fileName)
{
	qDebug() << "openFile : " << fileName;
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

void MainWindow::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(frame, tcType), QString::number(_stripClock->rate()));
	ui->statusbar->showMessage(message);
}

void MainWindow::onRateChanged(PhRate rate)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(_stripClock->frame(), _stripClock->timeCodeType()), QString::number(rate));
	ui->statusbar->showMessage(message);
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
	openVideoFile(fileName); // TODO: show error in case of error
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


void MainWindow::on_actionSet_Time_Code_triggered()
{
	PhTimeCodeDialog dlg(_stripClock->timeCodeType(), _stripClock->frame());
	if(dlg.exec() == QDialog::Accepted)
		ui->videoView->setFrameStamp(dlg.frame());

}
