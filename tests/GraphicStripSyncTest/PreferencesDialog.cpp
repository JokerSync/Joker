#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(GraphicStripSyncTestSettings *settings, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog),
	_settings(settings)
{
	ui->setupUi(this);

	if(_settings->useQuarterFrame()) {
		ui->useQuarterFrameRadioButton->setChecked(true);
		ui->delaySpinBox->setValue(_settings->screenDelay() / 20);
	}
	else
		ui->delaySpinBox->setValue(_settings->screenDelay());


	ui->delaySpinBox->setFocus();
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::on_delaySpinBox_valueChanged(int delay)
{
	if(_settings->useQuarterFrame())
		_settings->setScreenDelay(delay * 20);
	else
		_settings->setScreenDelay(delay);
}

void PreferencesDialog::on_useQuarterFrameRadioButton_toggled(bool checked)
{
	_settings->setUseQuarterFrame(checked);
	if(checked)
		ui->delaySpinBox->setValue(_settings->screenDelay() / 20);
	else
		ui->delaySpinBox->setValue(_settings->screenDelay());
	ui->delaySpinBox->selectAll();
}
