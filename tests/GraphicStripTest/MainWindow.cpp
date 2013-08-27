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

	connect(_clock, SIGNAL(frameChanged()), this, SLOT(onFrameOrRateChanged()));
	connect(_clock, SIGNAL(rateChanged()), this, SLOT(onFrameOrRateChanged()));
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

void MainWindow::onFrameOrRateChanged()
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(_clock->frame(), _clock->getTCType()), QString::number(_clock->rate()));
	ui->statusbar->showMessage(message);
}

