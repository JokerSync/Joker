/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QDir>
#include "PreferencesDialog.h"
#include "PhDebug.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QSettings *settings, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog),
	_settings(settings)
{
	// Load the ui
	ui->setupUi(this);
	this->setMaximumHeight(this->height());
	this->setMaximumWidth(this->width());
	ui->tabWidget->setCurrentIndex(0);
	this->setFocus();

	// Load the old settings
	_oldUseQuarterFrame = _settings->value("useQuarterFrame", false).toBool();
	_oldDelay = _settings->value("delay", 0).toInt();
	_oldStripHeight = _settings->value("stripHeight", 0.25f).toFloat();
	_oldSonyAutoConnect = _settings->value("sonyAutoConnect", true).toBool();
	_oldOpenLastFile = _settings->value("openLastFile", true).toBool();
	_oldStartFullScreen = _settings->value("startFullScreen", false).toBool();
	_oldSpeed = _settings->value("speed", 12).toInt();
	_oldBolness = _settings->value("boldness", 0).toInt();
	_oldFont = _settings->value("StripFontName", "").toString();

	ui->sliderBoldness->setValue(_oldBolness);
	ui->spinBoxSpeed->setValue(_oldSpeed);
	if(_oldUseQuarterFrame)
	{
		ui->radioButtonQF->setChecked(true);
		ui->spinBoxDelay->setValue(_oldDelay / 10);
	}
	else
	{
		ui->radioButtonMS->setChecked(true);
		ui->spinBoxDelay->setValue(_oldDelay);
	}

	ui->sliderStripHeight->setValue(ui->sliderStripHeight->maximum() * _oldStripHeight);
	ui->cBoxLastFile->setChecked(_oldOpenLastFile);
	ui->cBoxFullscreen->setChecked(_oldStartFullScreen);
	ui->cBoxSonyAutoconnect->setChecked(_oldSonyAutoConnect);



	//Set the fonts
	QStringList userFontList, systemFontList;
	QDir systemFont("/Library/Fonts/");
	QDir userFont("~/Library/Fonts/");



	QStringList filters;
	filters.append("*.ttf");
	systemFont.setNameFilters(filters);
	userFont.setNameFilters(filters);
	userFontList = userFont.entryList();
	systemFontList = systemFont.entryList();


	foreach(QString fontName, systemFontList)
	{
		fontList[fontName.split(".").first()] = "/Library/Fonts/" + fontName;
	}
	foreach(QString fontName, userFontList)
	{
		fontList[fontName.split(".").first()] = "~/Library/Fonts/" + fontName;
	}

	foreach(QString fontName, fontList.keys())
	{
		ui->listWidgetFont->addItem(fontName);
	}
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}


void PreferencesDialog::on_buttonBox_accepted()
{
	close();
}

void PreferencesDialog::on_buttonBox_rejected()
{
	_settings->setValue("useQuarterFrame", _oldUseQuarterFrame);
	_settings->setValue("delay", _oldDelay);
	_settings->setValue("stripHeight", _oldStripHeight);
	_settings->setValue("sonyAutoConnect", _oldSonyAutoConnect);
	_settings->setValue("openLastFile", _oldOpenLastFile);
	_settings->setValue("startFullScreen", _oldStartFullScreen);
	_settings->setValue("speed", _oldSpeed);
	_settings->setValue("boldness", _oldBolness);
	_settings->setValue("StripFontName", _oldFont);


	close();
}


void PreferencesDialog::on_spinBoxDelay_valueChanged(int delay)
{
	if(_settings->value("useQuarterFrame", false).toBool())
		_settings->setValue("delay", delay * 10);
	else
		_settings->setValue("delay", delay);
}

void PreferencesDialog::on_spinBoxSpeed_valueChanged(int speed)
{
	_settings->setValue("speed", speed);
}

void PreferencesDialog::on_radioButtonQF_toggled(bool checked)
{
	_settings->setValue("useQuarterFrame", checked);
	if(checked)
		ui->spinBoxDelay->setValue(_settings->value("delay", 0).toInt() / 10);
	else
		ui->spinBoxDelay->setValue(_settings->value("delay", 0).toInt());
	ui->spinBoxDelay->selectAll();
}

void PreferencesDialog::on_sliderStripHeight_valueChanged(int position)
{
	_settings->setValue("stripHeight", ((float)position / ui->sliderStripHeight->maximum()));
}

void PreferencesDialog::on_cBoxSonyAutoconnect_toggled(bool checked)
{
	_settings->setValue("sonyAutoConnect", checked);
}

void PreferencesDialog::on_cBoxLastFile_toggled(bool checked)
{
	_settings->setValue("openLastFile", checked);
}

void PreferencesDialog::on_cBoxFullscreen_toggled(bool checked)
{
	_settings->setValue("startFullScreen", checked);
}

void PreferencesDialog::on_sliderBoldness_valueChanged(int value)
{
	_settings->setValue("boldness", value);
}

void PreferencesDialog::on_lineEditFilter_textEdited(const QString &arg1)
{
	ui->listWidgetFont->clear();
	foreach(QString fontName, fontList.keys())
	{
		if(fontName.contains(&arg1, Qt::CaseInsensitive))
			ui->listWidgetFont->addItem(fontName);
	}
}


void PreferencesDialog::on_listWidgetFont_itemClicked(QListWidgetItem *item)
{
	_settings->setValue("StripFontFile", fontList[item->text()]);
}

void PreferencesDialog::on_listWidgetFont_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	_settings->setValue("StripFontFile", fontList[current->text()]);
}
