#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QSettings *settings, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog),
	_settings(settings)
{
	ui->setupUi(this);

	if(_settings->value("useQuarterFrame", false).toBool())
	{
		ui->useQuarterFrameRadioButton->setChecked(true);
		ui->delaySpinBox->setValue(_settings->value("delay", 0).toInt() / 20);
	}
	else
		ui->delaySpinBox->setValue(_settings->value("delay", 0).toInt());


	ui->delaySpinBox->setFocus();
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::on_delaySpinBox_valueChanged(int delay)
{
	if(_settings->value("useQuarterFrame", false).toBool())
		_settings->setValue("delay", delay * 20);
	else
		_settings->setValue("delay", delay);
}

void PreferencesDialog::on_useQuarterFrameRadioButton_toggled(bool checked)
{
	_settings->setValue("useQuarterFrame", checked);
	if(checked)
		ui->delaySpinBox->setValue(_settings->value("delay", 0).toInt() / 20);
	else
		ui->delaySpinBox->setValue(_settings->value("delay", 0).toInt());
	ui->delaySpinBox->selectAll();
}
