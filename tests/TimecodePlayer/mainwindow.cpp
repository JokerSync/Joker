#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_clock = new PhClock;
	PhTimeCodeType timecodeType = PhTimeCodeType25;
	PhFrame lengthFile = 7500;
	ui->mediaController->setClock(_clock);
	PhFrame firstFrame = _clock->frame();
	ui->mediaController->setMediaLength(lengthFile);
	ui->mediaController->setTCType(timecodeType);
	ui->mediaController->setFirstFrame(firstFrame);
	PhFrame fps = PhTimeCode::getFps(timecodeType);

	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(1000/fps);
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::updateFrame()
{
	PhFrame fps = PhTimeCode::getFps(_clock->getTCType());
	_clock->tick(fps);
}

