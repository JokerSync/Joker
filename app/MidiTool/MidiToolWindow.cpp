/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "MidiToolWindow.h"
#include "ui_MidiToolWindow.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PreferencesDialog.h"

MidiToolWindow::MidiToolWindow(MidiToolSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MidiToolWindow),
	_settings(settings),
	_mtcReader(PhTimeCodeType25),
	_mtcWriter(PhTimeCodeType25),
	_currentDigit(0),
	_lastFrame(-1),
	_frameDelta(-1),
	_lastRate(-1)
{
	ui->setupUi(this);

	on_checkBoxReadMTC_clicked(_settings->readMTC());
	on_checkBoxWriteMTC_clicked(_settings->writeMTC());

	_mtcWriter.clock()->setFrame(_settings->firstFrame());
	ui->widgetMaster->setMediaLength(_settings->length());
	ui->widgetMaster->setFirstFrame(_settings->firstFrame());
	ui->widgetMaster->setClock(_mtcWriter.clock());

	connect(&_clockTimer, &QTimer::timeout, this, &MidiToolWindow::onTick);

	_clockTimer.start(10);

	connect(_mtcReader.clock(), &PhClock::timeChanged, this, &MidiToolWindow::onReaderTimeChanged);
	connect(_mtcReader.clock(), &PhClock::rateChanged, this, &MidiToolWindow::onReaderRateChanged);
	connect(_mtcReader.clock(), &PhClock::tcTypeChanged, this, &MidiToolWindow::onReaderTCTypeChanged);
	onReaderTCTypeChanged(_mtcReader.clock()->timeCodeType());
}

MidiToolWindow::~MidiToolWindow()
{
	delete ui;
}

void MidiToolWindow::on_actionSet_TC_In_triggered()
{
//	PhTimeCodeDialog dlg(_ltcWriter.clock()->timeCodeType(), ui->widgetMaster->getFirstFrame());
//	if(dlg.exec()) {
//		ui->widgetMaster->setFirstFrame(dlg.frame());
//		_ltcWriter.clock()->setFrame(ui->widgetMaster->getFirstFrame());
//		updateInfos();
//	}
}

void MidiToolWindow::on_actionSet_TC_Out_triggered()
{
//	PhTimeCodeDialog dlg(_ltcWriter.clock()->timeCodeType(), ui->widgetMaster->getFirstFrame() + ui->widgetMaster->getMediaLength());
//	if(dlg.exec()) {
//		if(dlg.frame() > ui->widgetMaster->getFirstFrame())
//			ui->widgetMaster->setMediaLength(dlg.frame() - ui->widgetMaster->getFirstFrame());
//		else
//			PHDEBUG << "Can't set a TC Out inferior to TC In";
//		updateInfos();
//	}
}

void MidiToolWindow::on_actionPreferences_triggered()
{
	_mtcReader.close();
	_mtcWriter.close();
	PreferencesDialog dlg(_settings);
	dlg.exec();
	on_checkBoxReadMTC_clicked(_settings->readMTC());
	on_checkBoxWriteMTC_clicked(_settings->writeMTC());
}

void MidiToolWindow::on_checkBoxWriteMTC_clicked(bool checked)
{
	ui->checkBoxWriteMTC->setChecked(checked);
	ui->groupBoxWriter->setEnabled(checked);
	_settings->setWriteMTC(checked);
	if(checked) {
		if(!_mtcWriter.open(_settings->midiOutputPortName())) {
			QMessageBox::critical(this, "Error", "Unable to open " + _settings->midiOutputPortName());
			on_checkBoxWriteMTC_clicked(false);
		}
	}
	else
		_mtcWriter.close();
}

void MidiToolWindow::on_checkBoxReadMTC_clicked(bool checked)
{
	ui->checkBoxReadMTC->setChecked(checked);
	ui->groupBoxReader->setEnabled(checked);
	_settings->setReadMTC(checked);
	if(checked) {
		if(!_mtcReader.open(_settings->midiInputPortName())) {
			QMessageBox::critical(this, "Error", "Unable to create " + _settings->midiInputPortName());
			on_checkBoxReadMTC_clicked(false);
		}
	}
	else
		_mtcReader.close();
}

void MidiToolWindow::onReaderTimeChanged(PhTime time)
{
	ui->labelReaderTimeCode->setText(PhTimeCode::stringFromTime(time, _mtcReader.clock()->timeCodeType()));
	int delay = (_mtcWriter.clock()->time() - _mtcReader.clock()->time()) / 24;
	ui->labelDelay->setText(QString("%0 ms").arg(delay));
}

void MidiToolWindow::onReaderRateChanged(PhRate rate)
{
	QString s = QString("%1x since %2")
	            .arg(rate)
	            .arg(PhTimeCode::stringFromTime(_mtcReader.clock()->time(), _mtcReader.clock()->timeCodeType()));
	ui->readInfoLabel->setText(s);
}

void MidiToolWindow::onReaderTCTypeChanged(PhTimeCodeType tcType)
{
	ui->fpsLabel->setText(QString("%0 fps").arg(PhTimeCode::getAverageFps(tcType)));
}

void MidiToolWindow::onTick()
{
	_mtcWriter.clock()->tick(100);
}
