#include "TimecodePlayerWindow.h"
#include "ui_TimecodePlayerWindow.h"

SonyToolWindow::SonyToolWindow(QWidget *parent) :
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

SonyToolWindow::~SonyToolWindow()
{
	delete ui;
}

void SonyToolWindow::updateFrame()
{
	_clock->tick(100);
}
