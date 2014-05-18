#include <QMessageBox>
#include <QFileDialog>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PreferencesDialog.h"

#include "GraphicStripSyncTestWindow.h"
#include "ui_GraphicStripSyncTestWindow.h"

GraphicStripSyncTestWindow::GraphicStripSyncTestWindow(GraphicStripSyncTestSettings *settings) :
	PhDocumentWindow(settings),
	ui(new Ui::GraphicStripSyncTestWindow),
	_settings(settings),
	_strip(settings),
	_doc(_strip.doc()),
	_clock(_strip.clock()),
	_sonySlave(PhTimeCodeType25, _settings)
{
	ui->setupUi(this);
	ui->stripView->setGraphicSettings(_settings);
	_clockSynchroniser.setStripClock(_clock);

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));

	connect(_clock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));

	if(_sonySlave.open()) {
		_clock = _sonySlave.clock();
		_clockSynchroniser.setSonyClock(_clock);
		connect(ui->stripView, &PhGraphicView::beforePaint, &_sonySlave, &PhSonySlaveController::checkVideoSync);
	}
	else
		QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony slave");

	connect(ui->stripView, &PhGraphicView::beforePaint, _clock, &PhClock::tick);
	connect(ui->stripView, &PhGraphicView::paint, this, &GraphicStripSyncTestWindow::onPaint);
}

GraphicStripSyncTestWindow::~GraphicStripSyncTestWindow()
{
	delete ui;
}

bool GraphicStripSyncTestWindow::openDocument(QString fileName)
{
	PHDEBUG << "openFile : " << fileName;
	if(QFile::exists(fileName)) {
		if(_doc->openStripFile(fileName)) {
			_clock->setTimeCodeType(_doc->timeCodeType());
			_clock->setTime(_doc->lastTime());
			setCurrentDocument(fileName);
		}
	}
}

QMenu *GraphicStripSyncTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

void GraphicStripSyncTestWindow::onOpenFile()
{
	QFileDialog dlg(this, "Open...", "", "Rythmo files (*.detx)");
	if(dlg.exec()) {
		QString fileName = dlg.selectedFiles()[0];
		openDocument(fileName);
	}
}

void GraphicStripSyncTestWindow::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(frame, tcType), QString::number(_clock->rate()));
	ui->statusbar->showMessage(message);
}

void GraphicStripSyncTestWindow::onRateChanged(PhRate rate)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(_clock->frame(), _clock->timeCodeType()), QString::number(rate));
	ui->statusbar->showMessage(message);
}

void GraphicStripSyncTestWindow::on_actionPlay_pause_triggered()
{
	if(_clock->rate() == 0.0)
		_clock->setRate(1.0);
	else
		_clock->setRate(0.0);
}

void GraphicStripSyncTestWindow::on_actionPlay_backward_triggered()
{
	_clock->setRate(-1.0);
}

void GraphicStripSyncTestWindow::on_actionStep_forward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() + 1);
}

void GraphicStripSyncTestWindow::on_actionStep_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() - 1);
}

void GraphicStripSyncTestWindow::on_actionStep_time_forward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() + 1);
}

void GraphicStripSyncTestWindow::on_actionStep_time_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() - 1);
}

void GraphicStripSyncTestWindow::on_action_3_triggered()
{
	_clock->setRate(-3.0);
}

void GraphicStripSyncTestWindow::on_action_1_triggered()
{
	_clock->setRate(-1.0);
}

void GraphicStripSyncTestWindow::on_action_0_5_triggered()
{
	_clock->setRate(-0.5);
}

void GraphicStripSyncTestWindow::on_action0_triggered()
{
	_clock->setRate(0.0);
}

void GraphicStripSyncTestWindow::on_action0_5_triggered()
{
	_clock->setRate(0.5);
}

void GraphicStripSyncTestWindow::on_action1_triggered()
{
	_clock->setRate(1.0);
}

void GraphicStripSyncTestWindow::on_action3_triggered()
{
	_clock->setRate(3.0);
}

void GraphicStripSyncTestWindow::on_actionGo_To_triggered()
{
	PhTimeCodeDialog dlg(_clock->timeCodeType(), _clock->frame());
	if(dlg.exec() == QDialog::Accepted)
		_clock->setFrame(dlg.frame());

}

void GraphicStripSyncTestWindow::on_actionPreferences_triggered()
{
	PreferencesDialog dlg(_settings);
	if(dlg.exec() == QDialog::Accepted)
		PHDEBUG << "accepted";
	else
		PHDEBUG << "canceled";
}

void GraphicStripSyncTestWindow::onPaint(int width, int height)
{
	int h = height;
	if(_settings)
		h = height * _settings->stripHeight();
	_strip.draw(0, height - h, width, h);
	foreach(QString info, _strip.infos()) {
		ui->stripView->addInfo(info);
	}
}
