#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_stripView = ui->stripView;
	_doc = _stripView->doc();
	_clock = _stripView->clock();

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));

	connect(_clock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameOrRateChanged(PhFrame, PhTimeCodeType)));
//	connect(_clock, SIGNAL(rateChanged()), this, SLOT(onFrameOrRateChanged()));
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
			_clock->setTCType(_doc->getTCType());
			_clock->setFrame(_doc->getLastFrame());
			_clock->setRate(1);
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

void MainWindow::onFrameOrRateChanged(PhFrame frame, PhTimeCodeType tcType)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(frame, tcType), QString::number(_clock->rate()));
	ui->statusbar->showMessage(message);
}


void MainWindow::on_actionPlay_pause_triggered()
{
	if(_clock->rate() == 0.0)
		_clock->setRate(1.0);
	else
		_clock->setRate(0.0);
}

void MainWindow::on_actionPlay_backward_triggered()
{
    _clock->setRate(-1.0);
}

void MainWindow::on_actionStep_forward_triggered()
{
    _clock->setRate(0.0);
	_clock->setFrame(_clock->frame() + 1);
}

void MainWindow::on_actionStep_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() - 1);
}
