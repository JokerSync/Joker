/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent, SonyToolSettings *settings) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog),
	_settings(settings)
{
	ui->setupUi(this);

	ui->videoSyncFrameRateComboBox->setCurrentIndex(_settings->sonyMasterVideoSyncTimeCodeType());
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::accept()
{
	PhTimeCodeType tcType;
	switch (ui->videoSyncFrameRateComboBox->currentIndex()) {
	case 0:
		tcType = PhTimeCodeType2398;
		break;
	case 1:
		tcType = PhTimeCodeType24;
		break;
	case 2:
		tcType = PhTimeCodeType25;
		break;
	case 3:
		tcType = PhTimeCodeType2997;
		break;
	case 4:
	default:
		tcType = PhTimeCodeType30;
		break;
	}
	_settings->setSonyMasterVideoSyncTimeCodeType(tcType);
	_settings->setSonySlaveVideoSyncTimeCodeType(tcType);

	QDialog::accept();
}
