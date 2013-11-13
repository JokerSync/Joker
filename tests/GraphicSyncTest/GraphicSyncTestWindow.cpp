#include "GraphicSyncTestWindow.h"
#include "ui_GraphicSyncTestWindow.h"

GraphicSyncTestWindow::GraphicSyncTestWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GraphicSyncTestWindow),
	_settings("Phonations", "GraphicSyncTest"),
	_sony(PhTimeCodeType25, &_settings)
{
	ui->setupUi(this);

	connect(ui->graphicView, SIGNAL(beforePaint(int)), this, SLOT(beforePaint(int)));
	_sony.open();
	_sony.clock()->setRate(1);
}

GraphicSyncTestWindow::~GraphicSyncTestWindow()
{
	delete ui;
}

void GraphicSyncTestWindow::beforePaint(int frequency)
{
	_sony.checkVideoSync();
	PHDEBUG << frequency << ui->graphicView->refreshRate() << _sony.clock()->frame();
}
