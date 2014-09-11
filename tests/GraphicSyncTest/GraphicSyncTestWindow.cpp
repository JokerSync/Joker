#include "GraphicSyncTestWindow.h"
#include "ui_GraphicSyncTestWindow.h"

GraphicSyncTestWindow::GraphicSyncTestWindow(GraphicSyncTestSettings *settings) :
	QMainWindow(NULL),
	ui(new Ui::GraphicSyncTestWindow),
	_settings(settings),
	_sony(settings),
	_lastTime(0),
	_count(0)
{
	ui->setupUi(this);

	_sony.open(true);
	_sony.clock()->setRate(1);

	connect(ui->graphicView, SIGNAL(beforePaint(int)), this, SLOT(beforePaint(int)));

	_time.start();
}

GraphicSyncTestWindow::~GraphicSyncTestWindow()
{
	delete ui;
}

void GraphicSyncTestWindow::beforePaint(int frequency)
{
	int elapsed = _time.elapsed() - _lastTime;
	_lastTime = _time.elapsed();
//	int refreshRate = ui->graphicView->refreshRate();
	_sony.checkVideoSync();
//	_sony.clock()->tick(frequency);
	float ratio = _count++;
	if(_sony.clock()->time() > 0)
		ratio /= _sony.clock()->time();
	QString s = " ";
	if(elapsed < 10 )
		s = "<";
	else if(elapsed > 30)
		s = ">";
//	else if (_count % 60)
//		return;

//	PHDEBUG << s << elapsed << refreshRate << _sony.clock()->frame() << _count << ratio;
//	PHDEBUG << s << _sony.clock()->frame() * 1000.0f / _time.elapsed();
	ui->graphicView->setText(_sony.clock()->timeCode(_sony.timeCodeType()));
}
