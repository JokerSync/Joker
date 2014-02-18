#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMessageBox>
#include <QFileDialog>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PreferencesDialog.h"

MainWindow::MainWindow(QSettings * settings, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_settings(settings),
	_sonySlave(PhTimeCodeType25, _settings)
{
	ui->setupUi(this);
	_strip = ui->stripView->strip();
	_strip->setSettings(_settings);
	_doc = _strip->doc();
	_clock = _strip->clock();
	_clockSynchroniser.setStripClock(_clock);

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));

	connect(_clock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));

	if(_sonySlave.open()) {
		_clock = _sonySlave.clock();
		_clockSynchroniser.setSonyClock(_clock);
		connect(ui->stripView, SIGNAL(beforePaint(int)), &_sonySlave, SLOT(checkVideoSync(int)));
	}
	else
		QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony slave");
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openFile(QString fileName)
{
	PHDEBUG << "openFile : " << fileName;
	if(QFile::exists(fileName)) {
		if(_doc->openStripFile(fileName)) {
			_clock->setTimeCodeType(_doc->getTCType());
			_clock->setFrame(_doc->getLastFrame());
			this->setWindowTitle(fileName);
			if(_settings)
				_settings->setValue("lastFile", fileName);
		}
	}
}

void MainWindow::onOpenFile()
{
	QFileDialog dlg(this, "Open...", "", "Rythmo files (*.detx)");
	if(dlg.exec()) {
		QString fileName = dlg.selectedFiles()[0];
		openFile(fileName);
	}
}

void MainWindow::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(frame, tcType), QString::number(_clock->rate()));
	ui->statusbar->showMessage(message);
}

void MainWindow::onRateChanged(PhRate rate)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(_clock->frame(), _clock->timeCodeType()), QString::number(rate));
	ui->statusbar->showMessage(message);
}

void MainWindow::on_actionPlay_pause_triggered()
{
	if(_clock->rate() == 0.0)
		_clock->setRate(1.0);
	else
		_clock->setRate(0.0);
}

void MainWindow::on_actionPlay_backward_triggered()
{
	_clock->setRate(-1.0);
}

void MainWindow::on_actionStep_forward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() + 1);
}

void MainWindow::on_actionStep_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() - 1);
}

void MainWindow::on_actionStep_time_forward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() + 1);
}

void MainWindow::on_actionStep_time_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() - 1);
}

void MainWindow::on_action_3_triggered()
{
	_clock->setRate(-3.0);
}

void MainWindow::on_action_1_triggered()
{
	_clock->setRate(-1.0);
}

void MainWindow::on_action_0_5_triggered()
{
	_clock->setRate(-0.5);
}

void MainWindow::on_action0_triggered()
{
	_clock->setRate(0.0);
}

void MainWindow::on_action0_5_triggered()
{
	_clock->setRate(0.5);
}

void MainWindow::on_action1_triggered()
{
	_clock->setRate(1.0);
}

void MainWindow::on_action3_triggered()
{
	_clock->setRate(3.0);
}

void MainWindow::on_actionGo_To_triggered()
{
	PhTimeCodeDialog dlg(_clock->timeCodeType(), _clock->frame());
	if(dlg.exec() == QDialog::Accepted)
		_clock->setFrame(dlg.frame());

}

void MainWindow::on_actionPreferences_triggered()
{
	PreferencesDialog dlg(_settings);
	if(dlg.exec() == QDialog::Accepted)
		PHDEBUG << "accepted";
	else
		PHDEBUG << "canceled";
}
