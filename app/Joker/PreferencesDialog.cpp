#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include "PhTools/PhDebug.h"

PreferencesDialog::PreferencesDialog(QSettings *settings, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog),
	_settings(settings)
{
	ui->setupUi(this);

	_oldUseQuarterFrame = _settings->value("useQuarterFrame", false).toBool();
	_oldDelay = _settings->value("delay", 0).toInt();
	_oldStripHeight = _settings->value("stripHeight", 0.25f).toFloat();
	if(_oldUseQuarterFrame)
	{
		ui->useQuarterFrameRadioButton->setChecked(true);
		ui->delaySpinBox->setValue(_oldDelay / 20);
	}
	else
		ui->delaySpinBox->setValue(_oldDelay);

	ui->delaySpinBox->setFocus();

	ui->stripHeightSlider->setValue(ui->stripHeightSlider->maximum() * _oldStripHeight);
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::reject()
{
	_settings->setValue("useQuarterFrame", _oldUseQuarterFrame);
	_settings->setValue("delay", _oldDelay);
	_settings->setValue("stripHeight", _oldStripHeight);
	QDialog::reject();
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

void PreferencesDialog::on_stripHeightSlider_sliderMoved(int position)
{
	PHDEBUG;
	_settings->setValue("stripHeight", ((float)position / ui->stripHeightSlider->maximum()));
}
