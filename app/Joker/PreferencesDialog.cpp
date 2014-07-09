/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QMessageBox>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include "ui_PreferencesDialog.h"
#include "PreferencesDialog.h"
#include "PhTools/PhDebug.h"
#include "PhSync/PhLtcReader.h"

PreferencesDialog::PreferencesDialog(JokerSettings *settings, QWidget *parent) :
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
	_oldUseQuarterFrame = _settings->useQuarterFrame();
	_oldDelay = _settings->screenDelay();
	_oldStripHeight = _settings->stripHeight();
	_oldHorizontalTimePerPixel = _settings->horizontalTimePerPixel();
	_oldBolness = _settings->textBoldness();
	_oldFont = _settings->textFontFile();
	_oldDisplayTC = _settings->displayTC();
	_oldDisplayNextTC = _settings->displayNextTC();
	_oldDisplayNextText = _settings->displayNextText();
	_oldDisplayTitle = _settings->displayTitle();
	_oldDisplayLoop = _settings->displayLoop();
	_oldSyncProtocol = _settings->synchroProtocol();
	_oldLTCInput = _settings->ltcInputDevice();

	ui->sliderBoldness->setValue(_oldBolness);
	ui->spinBoxSpeed->setValue(_oldHorizontalTimePerPixel);
	if(_oldUseQuarterFrame) {
		ui->radioButtonQF->setChecked(true);
		ui->spinBoxDelay->setValue(_oldDelay / 10);
	}
	else {
		ui->radioButtonMS->setChecked(true);
		ui->spinBoxDelay->setValue(_oldDelay);
	}

	ui->sliderStripHeight->setValue(ui->sliderStripHeight->maximum() * _oldStripHeight);
	ui->cBoxDisplayTC->setChecked(_oldDisplayTC);
	ui->cBoxDisplayNextTC->setChecked(_oldDisplayNextTC);
	ui->cBoxDisplayNextText->setChecked(_oldDisplayNextText);
	ui->cBoxDisplayTitle->setChecked(_oldDisplayTitle);
	ui->cBoxDisplayLoop->setChecked(_oldDisplayLoop);

	//Setting the filters
	QStringList filters;
	filters.append("*.ttf");
	filters.append("*.TTF");

	// Adding the system font
	QStringList systemFontList;
	QDir systemFont(QStandardPaths::writableLocation(QStandardPaths::FontsLocation));
	systemFont.setNameFilters(filters);
	systemFontList = systemFont.entryList();
	foreach(QString fontName, systemFontList)
	{
		_fontList[fontName.split(".").first()] = systemFont.filePath(fontName);
	}

#if defined(Q_OS_MAC)
	//Set the user fonts
	QStringList userFontList;
	QString userDirectory = QDir::homePath();
	QDir userFont(userDirectory + "/Library/Fonts/");
	userFont.setNameFilters(filters);
	userFontList = userFont.entryList();
	foreach(QString fontName, userFontList)
	{
		_fontList[fontName.split(".").first()] = userFont.filePath(fontName);
	}

	//Set the mac fonts
	QStringList macOSFontList;
	QDir macOSFont("/Library/Fonts/");
	macOSFont.setNameFilters(filters);
	macOSFontList = macOSFont.entryList();
	foreach(QString fontName, macOSFontList)
	{
		_fontList[fontName.split(".").first()] = macOSFont.filePath(fontName);
	}
#endif

	// Adding the default font
	_fontList["SWENSON"] = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + "SWENSON.TTF";

	// _oldFont is : /Path/To/Font.ttf
	// So split with "/" then take last gives Font.ttf
	// Split with "." then take first, gives the name of the font
	QString oldFontName = _oldFont.split("/").last().split(".").first();
	foreach(QString fontName, _fontList.keys())
	{
		ui->listWidgetFont->addItem(fontName);
		if(fontName == oldFontName) {
			ui->listWidgetFont->item(ui->listWidgetFont->count() - 1)->setSelected(true);
			ui->listWidgetFont->setCurrentRow(ui->listWidgetFont->count() - 1);
		}
	}

	ui->listWidgetSync->addItem("LTC");

	ui->listWidgetSync->setCurrentRow(_oldSyncProtocol);

	if(_oldSyncProtocol == Synchronizer::Sony)
		showParamSony(true);
	else if(_oldSyncProtocol == Synchronizer::LTC)
		showParamLTC(true);
	else {
		showParamLTC(false);
		showParamSony(false);
	}

	//Set the language
	QDir appDirectory(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/");



	QStringList filtersLang;
	filtersLang.append("*.qm");
	appDirectory.setNameFilters(filtersLang);
	QStringList languageFileList = appDirectory.entryList();


	_langNameMap[""] = tr("<System default>");
	ui->cboBoxLang->addItem(_langNameMap[""], "");

	foreach(QString tradFile, languageFileList)
	{
		QFileInfo info(tradFile);
		QString lang = info.baseName();
		if(lang == "fr_FR")
			_langNameMap[lang] = tr("French");
		else
			_langNameMap[lang] = lang;
		ui->cboBoxLang->addItem(_langNameMap[lang], lang);
	}
	QString eng = "English";
	_langNameMap["English"] = eng;
	ui->cboBoxLang->addItem(_langNameMap["English"], eng);

	ui->cboBoxLang->setCurrentText(_langNameMap[_settings->language()]);
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::on_buttonBox_accepted()
{
	if(ui->cboBoxLang->currentData() != _settings->language()) {
		QMessageBox::warning(this, tr("Information"),
		                     tr("You change the language to \"%1\".\n"
		                        "You need to restart %2 to apply you changes.").arg(ui->cboBoxLang->currentText(), APP_NAME),
		                     QMessageBox::Ok,
		                     QMessageBox::Ok);
		_settings->setLanguage(ui->cboBoxLang->currentData().toString());
	}
	close();
}

void PreferencesDialog::on_buttonBox_rejected()
{
	_settings->setUseQuarterFrame(_oldUseQuarterFrame);
	_settings->setScreenDelay(_oldDelay);
	_settings->setStripHeight(_oldStripHeight);
	_settings->setHorizontalTimePerPixel(_oldHorizontalTimePerPixel);
	_settings->setTextBoldness(_oldBolness);
	_settings->setTextFontFile(_oldFont);
	_settings->setDisplayTC(_oldDisplayTC);
	_settings->setDisplayNextTC(_oldDisplayNextTC);
	_settings->setDisplayNextText(_oldDisplayNextText);
	_settings->setDisplayTitle(_oldDisplayTitle);
	_settings->setDisplayLoop(_oldDisplayLoop);
	_settings->setLTCInputDevice(_oldLTCInput);

	close();
}

void PreferencesDialog::on_spinBoxDelay_valueChanged(int delay)
{
	if(_settings->useQuarterFrame())
		_settings->setScreenDelay(delay * 10);
	else
		_settings->setScreenDelay(delay);
}

void PreferencesDialog::on_spinBoxSpeed_valueChanged(int speed)
{
	_settings->setHorizontalTimePerPixel(speed);
}

void PreferencesDialog::on_radioButtonQF_toggled(bool checked)
{
	_settings->setUseQuarterFrame(checked);
	if(checked)
		ui->spinBoxDelay->setValue(_settings->screenDelay() / 10);
	else
		ui->spinBoxDelay->setValue(_settings->screenDelay());
	ui->spinBoxDelay->selectAll();
}

void PreferencesDialog::on_sliderStripHeight_valueChanged(int position)
{
	_settings->setStripHeight(((float)position / ui->sliderStripHeight->maximum()));
}

void PreferencesDialog::on_sliderBoldness_valueChanged(int value)
{
	_settings->setTextBoldness(value);
}

void PreferencesDialog::on_lineEditFilter_textEdited(const QString &value)
{
	ui->listWidgetFont->clear();
	foreach(QString fontName, _fontList.keys())
	{
		if(fontName.contains(&value, Qt::CaseInsensitive))
			ui->listWidgetFont->addItem(fontName);
	}
}

void PreferencesDialog::on_listWidgetFont_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	Q_UNUSED(previous);
	if(current)
		_settings->setTextFontFile(_fontList[current->text()]);
}

void PreferencesDialog::on_cBoxDisplayTC_clicked()
{
	_settings->setDisplayTC(ui->cBoxDisplayTC->isChecked());
}

void PreferencesDialog::on_cBoxDisplayNextTC_clicked()
{
	_settings->setDisplayNextTC(ui->cBoxDisplayNextTC->isChecked());
}

void PreferencesDialog::on_cBoxDisplayNextText_clicked()
{
	_settings->setDisplayNextText(ui->cBoxDisplayNextText->isChecked());
}

void PreferencesDialog::on_cBoxDisplayTitle_clicked()
{
	_settings->setDisplayTitle(ui->cBoxDisplayTitle->isChecked());
}

void PreferencesDialog::on_cBoxDisplayLoop_clicked()
{
	_settings->setDisplayLoop(ui->cBoxDisplayLoop->isChecked());
}

void PreferencesDialog::on_listWidgetSync_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	Q_UNUSED(current);
	Q_UNUSED(previous);
	int protocol = ui->listWidgetSync->currentRow();
	switch(protocol) {
	case Synchronizer::Sony:
		showParamSony(true);
		break;
	case Synchronizer::LTC:
		showParamLTC(true);
		break;
	default:
		showParamLTC(false);
		showParamSony(false);
		break;
	}
	_settings->setSynchroProtocol(protocol);
}

void PreferencesDialog::showParamLTC(bool show)
{
	if(show) {
		ui->listWidgetInputs->clear();
		ui->listWidgetInputs->setVisible(1);
		ui->lblInputs->setVisible(1);
		showParamSony(false);
		ui->listWidgetInputs->addItems(PhLtcReader::inputList());
		foreach(QString inputName, PhLtcReader::inputList()) {
			PHDEBUG << inputName;
		}

		if(ui->listWidgetInputs->findItems(_settings->ltcInputDevice(), Qt::MatchExactly).count() > 0)
			ui->listWidgetInputs->findItems(_settings->ltcInputDevice(), Qt::MatchExactly).first()->setSelected(1);
	}
	else {
		ui->lblInputs->setVisible(0);
		ui->listWidgetInputs->setVisible(0);
	}
}

void PreferencesDialog::showParamSony(bool show)
{
	if(show) {
		ui->spinBoxSonyHighSpeed->setVisible(1);
		ui->lineEditSonyID->setVisible(1);
		ui->lblSonyHighSpeed->setVisible(1);
		ui->lblSonyID->setVisible(1);
		showParamLTC(false);
	}
	else {
		ui->spinBoxSonyHighSpeed->setVisible(0);
		ui->lineEditSonyID->setVisible(0);
		ui->lblSonyHighSpeed->setVisible(0);
		ui->lblSonyID->setVisible(0);
	}
}


void PreferencesDialog::on_listWidgetInputs_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	Q_UNUSED(previous);
	_settings->setLTCInputDevice(current->text());
}

