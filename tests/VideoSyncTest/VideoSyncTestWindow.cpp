#include "VideoSyncTestWindow.h"
#include "ui_VideoSyncTestWindow.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>

VideoSyncTestWindow::VideoSyncTestWindow(VideoSyncTestSettings *settings)
	: PhDocumentWindow(settings),
	ui(new Ui::VideoSyncTestWindow),
	_settings(settings)
{
	ui->setupUi(this);
	_videoEngine.setSettings(_settings);
	ui->_videoView->setEngine(&_videoEngine);

	ui->mediaController->setClock(_videoEngine.clock());

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));
}


VideoSyncTestWindow::~VideoSyncTestWindow()
{
	delete ui;
}

bool VideoSyncTestWindow::openDocument(QString fileName)
{
	if (_videoEngine.open(fileName)) {
		ui->mediaController->setFirstFrame(_videoEngine.firstFrame());
		ui->mediaController->setMediaLength(_videoEngine.length());

		_videoEngine.clock()->setRate(0.0);

		setCurrentDocument(fileName);
		return true;
	}
	return false;
}

QMenu *VideoSyncTestWindow::recentDocumentMenu()
{
	ui->menuOpen_recent;
}

void VideoSyncTestWindow::onOpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"), QDir::homePath());
	openDocument(fileName);  // TODO: show error in case of error
}


