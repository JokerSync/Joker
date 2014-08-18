#include "TimecodePlayerWindow.h"
#include "ui_TimecodePlayerWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::TimecodePlayerWindow)
{
	ui->setupUi(this);
	ui->mediaController->setLength(PhTimeCode::timeFromString("00:01:00:00", PhTimeCodeType25));
	ui->mediaController->setClock(PhTimeCodeType25, &_clock);
	_clock.setTime(PhTimeCode::timeFromString("01:00:00:00", PhTimeCodeType25));

	_timer = new QTimer();
	connect(_timer, &QTimer::timeout, this, &MainWindow::updateFrame);
	_timer->start(10);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::updateFrame()
{
	_clock.tick(100);
}
