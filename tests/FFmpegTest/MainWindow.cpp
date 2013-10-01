#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <VideoRendererTypes.h>
#include <QFileDialog>

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

void MainWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open...");
	if(QFile::exists(fileName))
	{
		mpPlayer->play(fileName);
	}
}
