#include "GraphicSyncTestWindow.h"
#include "ui_GraphicSyncTestWindow.h"

GraphicSyncTestWindow::GraphicSyncTestWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GraphicSyncTestWindow),
	_settings("Phonations", "GraphicSyncTest"),
	_sony(PhTimeCodeType25, &_settings),
	_count(0)
{
	ui->setupUi(this);

	connect(ui->graphicView, SIGNAL(beforePaint(int)), this, SLOT(beforePaint(int)));
	_sony.open();
	_sony.clock()->setRate(1);
	_timer.start();
}

GraphicSyncTestWindow::~GraphicSyncTestWindow()
{
	delete ui;
}

void GraphicSyncTestWindow::beforePaint(int frequency)
{
	int elapsed = _timer.elapsed();
	int refreshRate = ui->graphicView->refreshRate();
	_timer.restart();
	_sony.checkVideoSync();
	float ratio = _count++;
	if(_sony.clock()->frame() > 0)
		ratio /= _sony.clock()->frame();
	QString s = " ";
	if(elapsed < 10 )
		s = "<";
	else if(elapsed > 30)
		s = ">";
	else if (_count % 60)
		return;

	PHDEBUG << s << elapsed << refreshRate << _sony.clock()->frame() << _count << ratio;
}
