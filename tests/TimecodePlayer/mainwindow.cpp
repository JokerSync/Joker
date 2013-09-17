#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_clock = new PhClock;


	_clock->setTCType(PhTimeCodeType25);
	ui->mediaController->setMediaLength(7500);
	ui->mediaController->setTCType(_clock->timeCodeType());
	ui->mediaController->setFirstFrame(_clock->frame());
	ui->mediaController->setClock(_clock);

	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(1000/PhTimeCode::getFps(_clock->timeCodeType()));
}


MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::updateFrame()
{
	_clock->tick(PhTimeCode::getFps(_clock->timeCodeType()));
}
