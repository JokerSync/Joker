#include "LTCToolWindow.h"
#include "ui_LTCToolWindow.h"

LTCToolWindow::LTCToolWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LTCToolWindow),
	_LTCWriter(PhTimeCodeType25)
{
	ui->setupUi(this);
	_LTCWriter.init();

	_clock = _LTCWriter.clock();
	ui->widget->setMediaLength(1 * 6 * 25);
	ui->widget->setTCType(_clock->timeCodeType());
	ui->widget->setFirstFrame(_clock->frame());
	ui->widget->setClock(_clock);



	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));

	connect(_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), &_LTCWriter, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	_timer->start(10);
}

LTCToolWindow::~LTCToolWindow()
{
	delete ui;
}

void LTCToolWindow::updateFrame()
{
	if (_clock->frame() < ui->widget->getMediaLength())
		_clock->tick(100);
	else if(ui->cBoxLoop->isChecked())
	{
		_clock->setFrame(ui->widget->getFirstFrame());
		_clock->tick(100);
	}
	else
	{
		_clock->setRate(0);
		_clock->setFrame(ui->widget->getFirstFrame());
	}

}
