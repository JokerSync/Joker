#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	PhTimeCodeType timecodeType = PhTimeCodeType25;
	PhFrame lengthFile = 7500;
	_clock = new PhClock;
	_clock->setFrame(PhTimeCode::frameFromString("01:00:00:00",timecodeType));
	PhFrame firstFrame = _clock->frame();
	ui->mediaController->setClock(_clock);
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

