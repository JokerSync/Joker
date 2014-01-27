/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QDir>
#include <QProcess>
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
	_oldOpenLastFile = _settings->value("openLastFile", true).toBool();
	_oldStartFullScreen = _settings->value("startFullScreen", false).toBool();
	_oldSpeed = _settings->value("speed", 12).toInt();
	_oldBolness = _settings->value("boldness", 0).toInt();
	_oldFont = _settings->value("StripFontFile", "").toString();
	_oldDeinterlace = _settings->value("videoDeinterlace", false).toBool();
	_oldDisplayTC = _settings->value("displayTC", true).toBool();
	_oldDisplayNextTC = _settings->value("displayNextTC", true).toBool();
	_oldDisplayNextText = _settings->value("displayNextText", true).toBool();
	_oldDisplayTitle = _settings->value("displayTitle", true).toBool();
	_oldDisplayLoop = _settings->value("displayLoop", false).toBool();
    _oldSyncProtocol = _settings->value("synchroProtocol").toInt();
    _oldLTCInput = _settings->value("ltcInputDevice").toString();

	_oldLogMask = _settings->value("logMask", 1).toInt();

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
	ui->cBoxDeinterlace->setChecked(_oldDeinterlace);
	ui->cBoxDisplayTC->setChecked(_oldDisplayTC);
	ui->cBoxDisplayNextTC->setChecked(_oldDisplayNextTC);
	ui->cBoxDisplayNextText->setChecked(_oldDisplayNextText);
	ui->cBoxDisplayTitle->setChecked(_oldDisplayTitle);
	ui->cBoxDisplayLoop->setChecked(_oldDisplayLoop);
	ui->lblPathToLogFile->setText("<a href=\""+ PhDebug::logLocation() +"\">" + PhDebug::logLocation() + "</a>");

	//Set the checkboxes of log
	foreach(QAbstractButton * btn, ui->buttonGroup->buttons())
	{
		connect(btn, SIGNAL(clicked()), this, SLOT(onLogMaskButtonClicked()));
		if((1 << btn->objectName().split("_").last().toInt()) & _oldLogMask)
			btn->setChecked(true);
	}

	//Set the fonts
	QStringList userFontList, systemFontList;
	QString userDirectory = QDir::homePath();
	QDir systemFont("/Library/Fonts/");
	QDir userFont(userDirectory + "/Library/Fonts/");



	QStringList filters;
	filters.append("*.ttf");
	filters.append("*.TTF");
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
		fontList[fontName.split(".").first()] = userDirectory + "/Library/Fonts/" + fontName;
	}

	// _oldFont is : /Path/To/Font.ttf
	// So split with "/" then take last gives Font.ttf
	// Split with "." then take first, gives the name of the font
	QString oldFontName = _oldFont.split("/").last().split(".").first();
	foreach(QString fontName, fontList.keys())
	{
		ui->listWidgetFont->addItem(fontName);
		if(fontName == oldFontName)
		{
			ui->listWidgetFont->item(ui->listWidgetFont->count() - 1)->setSelected(true);
			ui->listWidgetFont->setCurrentRow(ui->listWidgetFont->count() - 1);
		}
	}

    ui->listWidgetSync->setCurrentRow(_oldSyncProtocol);

    if(_oldSyncProtocol == VideoStripSynchronizer::Sony)
        showParamSony(true);
    else if(_oldSyncProtocol == VideoStripSynchronizer::LTC)
        showParamLTC(true);
    else
    {
        showParamLTC(false);
        showParamSony(false);
    }

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
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
	_settings->setValue("openLastFile", _oldOpenLastFile);
	_settings->setValue("startFullScreen", _oldStartFullScreen);
	_settings->setValue("speed", _oldSpeed);
	_settings->setValue("boldness", _oldBolness);
	_settings->setValue("StripFontName", _oldFont);
	_settings->setValue("videoDeinterlace", _oldDeinterlace);
	_settings->setValue("displayTC", _oldDisplayTC);
	_settings->setValue("displayNextTC", _oldDisplayNextTC);
	_settings->setValue("displayNextText", _oldDisplayNextText);
	_settings->setValue("displayTitle", _oldDisplayTitle);
	_settings->setValue("displayLoop", _oldDisplayLoop);
	_settings->setValue("logMask", _oldLogMask);
    _settings->setValue("ltcInputDevice", _oldLTCInput);
	PhDebug::setLogMask(_oldLogMask);

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

void PreferencesDialog::on_listWidgetFont_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	Q_UNUSED(previous);
	if(current)
		_settings->setValue("StripFontFile", fontList[current->text()]);
}

void PreferencesDialog::on_cBoxDeinterlace_clicked()
{
	_settings->setValue("videoDeinterlace", ui->cBoxDeinterlace->isChecked());
}

void PreferencesDialog::on_cBoxDisplayTC_clicked()
{
	_settings->setValue("displayTC", ui->cBoxDisplayTC->isChecked());
}

void PreferencesDialog::on_cBoxDisplayNextTC_clicked()
{
	_settings->setValue("displayNextTC", ui->cBoxDisplayNextTC->isChecked());
}

void PreferencesDialog::on_cBoxDisplayNextText_clicked()
{
	_settings->setValue("displayNextText", ui->cBoxDisplayNextText->isChecked());
}

void PreferencesDialog::on_cBoxDisplayTitle_clicked()
{
	_settings->setValue("displayTitle", ui->cBoxDisplayTitle->isChecked());
}

void PreferencesDialog::on_cBoxDisplayLoop_clicked()
{
	_settings->setValue("displayLoop", ui->cBoxDisplayLoop->isChecked());
}

void PreferencesDialog::on_pButtonReset_clicked()
{
	foreach(QAbstractButton * btn, ui->buttonGroup->buttons())
	{
		if(btn->objectName().split("_").last() != "0")
			btn->setChecked(false);
		else
			btn->setChecked(true);
	}
	onLogMaskButtonClicked();
}

void PreferencesDialog::on_lblPathToLogFile_linkActivated(const QString &link)
{
#if defined(Q_OS_MAC)
	QStringList args;
	args << "-e";
	args << "tell application \"Finder\"";
	args << "-e";
	args << "activate";
	args << "-e";
	args << "select POSIX file \""+ link +"\"";
	args << "-e";
	args << "end tell";
	QProcess::startDetached("osascript", args);
#endif
}

void PreferencesDialog::onLogMaskButtonClicked()
{
	int logMask = 0;
	//Set the checkboxes of log
	foreach(QAbstractButton * btn, ui->buttonGroup->buttons())
	{
		if(btn->isChecked())
			logMask += 1 << btn->objectName().split("_").last().toInt();
	}
	PhDebug::setLogMask(logMask);
	_settings->setValue("logMask", logMask);
}

void PreferencesDialog::on_listWidgetSync_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
	int protocol = ui->listWidgetSync->currentRow();
	switch(protocol)
	{
	case VideoStripSynchronizer::Sony:
        showParamSony(true);
		break;
	case VideoStripSynchronizer::LTC:
        showParamLTC(true);
		break;
	default:
        showParamLTC(false);
        showParamSony(false);
		break;
    }
    _settings->setValue("synchroProtocol", protocol);
}

void PreferencesDialog::showParamLTC(bool show)
{
    if(show)
    {
        ui->listWidgetInputs->clear();
        ui->listWidgetInputs->setVisible(1);
        ui->lblInputs->setVisible(1);
        showParamSony(false);
        ui->listWidgetInputs->addItems(PhLtcReader::inputList());
        if(ui->listWidgetInputs->findItems(_settings->value("ltcInputDevice", "").toString(), Qt::MatchExactly).count() > 0)
            ui->listWidgetInputs->findItems(_settings->value("ltcInputDevice", "").toString(), Qt::MatchExactly).first()->setSelected(1);
    }
    else
    {
        ui->lblInputs->setVisible(0);
        ui->listWidgetInputs->setVisible(0);
    }
}

void PreferencesDialog::showParamSony(bool show)
{
    if(show)
    {
        ui->spinBoxSonyHighSpeed->setVisible(1);
        ui->lineEditSonyID->setVisible(1);
        ui->lblSonyHighSpeed->setVisible(1);
        ui->lblSonyID->setVisible(1);
        showParamLTC(false);
    }
    else
    {
        ui->spinBoxSonyHighSpeed->setVisible(0);
        ui->lineEditSonyID->setVisible(0);
        ui->lblSonyHighSpeed->setVisible(0);
        ui->lblSonyID->setVisible(0);
    }
}


void PreferencesDialog::on_listWidgetInputs_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    _settings->setValue("ltcInputDevice", current->text());
}
