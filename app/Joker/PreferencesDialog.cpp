/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include <QDir>
#include <QProcess>
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

	_oldLogMask = _settings->logMask();

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
	if(!fontList["SWENSON"].isNull())
		fontList["SWENSON"] = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + "SWENSON.TTF";


	// _oldFont is : /Path/To/Font.ttf
	// So split with "/" then take last gives Font.ttf
	// Split with "." then take first, gives the name of the font
	QString oldFontName = _oldFont.split("/").last().split(".").first();
	foreach(QString fontName, fontList.keys())
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
	_settings->setLogMask(_oldLogMask);
	_settings->setLTCInputDevice(_oldLTCInput);
	PhDebug::setLogMask(_oldLogMask);

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
		_settings->setTextFontFile(fontList[current->text()]);
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
#else
#warning /// @todo Fix me
	Q_UNUSED(link);
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
	_settings->setLogMask(logMask);
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
