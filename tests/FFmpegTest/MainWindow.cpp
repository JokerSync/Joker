#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <VideoRendererTypes.h>
#include <QFileDialog>
#include <QMessageBox>

using namespace QtAV;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	mpPlayer = new AVPlayer(this);
	mpRenderer = VideoRendererFactory::create(VideoRendererId_Widget);

	mpPlayer->setRenderer(mpRenderer);
	ui->verticalLayout->addWidget(mpRenderer->widget());
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::openFile(QString fileName)
{
	return mpPlayer->play(fileName);
}

void MainWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open...");
	if(QFile::exists(fileName))
	{
		if(!openFile(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void MainWindow::on_actionPlay_pause_triggered()
{
	if(mpPlayer->isLoaded())
	{
		qreal speed = mpPlayer->speed();
		if(speed == 0)
			mpPlayer->play();
		else
			mpPlayer->pause(true);
	}
}
