#include "MainView.h"
#include "ui_MainView.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>

MainView::MainView(QSettings *settings)
	: QMainWindow(0),
	ui(new Ui::MainView),
	_settings(settings)
{
	ui->setupUi(this);
	_videoEngine.setSettings(_settings);
	ui->_videoView->setEngine(&_videoEngine);

	ui->mediaController->setClock(_videoEngine.clock());

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));
}


MainView::~MainView()
{
	delete ui;
}

bool MainView::openFile(QString fileName)
{
	if (_videoEngine.open(fileName)) {
#warning TODO read media length from video file
		ui->mediaController->setMediaLength(7500);
#warning TODO read first frame from video file
		ui->mediaController->setFirstFrame(0);

		_videoEngine.clock()->setRate(0.0);

		_settings->setValue("lastFile", fileName);
		return true;
	}
	return false;
}


void MainView::onOpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
	openFile(fileName);  // TODO: show error in case of error
}


