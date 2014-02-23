#include "TimecodePlayerWindow.h"
#include "ui_TimecodePlayerWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::TimecodePlayerWindow)
{
	ui->setupUi(this);
	_clock = new PhClock(PhTimeCodeType25);
	ui->mediaController->setMediaLength(7500);
	ui->mediaController->setTCType(_clock->timeCodeType());
	ui->mediaController->setFirstFrame(_clock->frame());
	ui->mediaController->setClock(_clock);

	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(10);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::updateFrame()
{
	_clock->tick(100);
}
