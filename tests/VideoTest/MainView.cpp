#include "MainView.h"
#include "ui_MainView.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>

MainView::MainView()
	: QMainWindow(0),
	  ui(new Ui::MainView)
{
	_clock = new PhClock(PhTimeCodeType25);
	ui->setupUi(this);
	ui->_videoView->setClock(_clock);

	ui->mediaController->setClock(_clock);

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));
	connect(&timer, SIGNAL(timeout()), ui->_videoView, SLOT(checkVideoPosition()));

	timer.start(10);
}

MainView::~MainView()
{
	delete ui;
}

bool MainView::openFile(QString fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
		ui->_videoView->open(fileName);
#warning TODO read media length from video file
		ui->mediaController->setMediaLength(7500);
#warning TODO read first frame from video file
		ui->mediaController->setFirstFrame(0);

		_clock->setRate(0.0);
		return true;
    }
	return false;
}


void MainView::onOpenFile()
{
	 QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
	 openFile(fileName); // TODO: show error in case of error
}


