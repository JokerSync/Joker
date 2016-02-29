/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QStandardPaths>
#include <QMessageBox>

#include "PhTools/PhDebug.h"
#include "PhSync/PhSynchronizer.h"
#ifdef USE_LTC
#include "PhLtc/PhLtcReader.h"
#endif // USE_LTC
#ifdef USE_MIDI
#include "PhMidi/PhMidiInput.h"
#include "PhMidi/PhMidiOutput.h"
#endif // USE_MIDI

#include "ui_PreferencesDialog.h"
#include "PreferencesDialog.h"

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

	// Save the old settings in case of cancel
	_oldScreenDelay = _settings->screenDelay();
	_oldSecondScreenDelay = _settings->videoSecondScreenDelay();
	_oldPipOffset = _settings->videoPictureInPictureOffset();
	_oldPipRatio = _settings->videoPictureInPictureRatio();
	_oldPipPositionRight = _settings->videoPictureInPicturePositionRight();
	_oldStripHeight = _settings->stripHeight();
	_oldHorizontalTimePerPixel = _settings->horizontalTimePerPixel();
	_oldBolness = _settings->textBoldness();
	_oldFont = _settings->textFontFile();

	ui->sliderBoldness->setValue(_oldBolness);
	ui->spinBoxSpeed->setValue(_oldHorizontalTimePerPixel);

	ui->mainScreenDelayspinBox->setValue(_oldScreenDelay);
	ui->secondScreenDelaySpinBox->setValue(_oldSecondScreenDelay);
	ui->pipOffsetSpinBox->setValue(_oldPipOffset);
	ui->pipRatioSlider->setValue(_oldPipRatio * 100);
	if(_oldPipPositionRight)
		ui->pipRightPositionRadioButton->setChecked(true);
	else
		ui->pipLeftPositionRadioButton->setChecked(true);

	ui->sliderStripHeight->setValue(ui->sliderStripHeight->maximum() * _oldStripHeight);

	//Setting the filters
	QStringList filters;
	filters.append("*.ttf");
	filters.append("*.TTF");

	// Adding the system font
	QStringList systemFontList;
	QDir systemFont(QStandardPaths::writableLocation(QStandardPaths::FontsLocation));
	systemFont.setNameFilters(filters);
	systemFontList = systemFont.entryList();
	foreach(QString fontName, systemFontList) {
		_fontList[fontName.split(".").first()] = systemFont.filePath(fontName);
	}

#if defined(Q_OS_MAC)
	//Set the user fonts
	QStringList userFontList;
	QString userDirectory = QDir::homePath();
	QDir userFont(userDirectory + "/Library/Fonts/");
	userFont.setNameFilters(filters);
	userFontList = userFont.entryList();
	foreach(QString fontName, userFontList) {
		_fontList[fontName.split(".").first()] = userFont.filePath(fontName);
	}

	//Set the mac fonts
	QStringList macOSFontList;
	QDir macOSFont("/Library/Fonts/");
	macOSFont.setNameFilters(filters);
	macOSFontList = macOSFont.entryList();
	foreach(QString fontName, macOSFontList) {
		_fontList[fontName.split(".").first()] = macOSFont.filePath(fontName);
	}
#endif

	// Adding the default font
	_fontList["Capella"] = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + "Cappella.ttf";

	// _oldFont is : /Path/To/Font.ttf
	// So split with "/" then take last gives Font.ttf
	// Split with "." then take first, gives the name of the font
	QString oldFontName = _oldFont.split("/").last().split(".").first();
	foreach(QString fontName, _fontList.keys()) {
		ui->listWidgetFont->addItem(fontName);
		if(fontName == oldFontName) {
			ui->listWidgetFont->item(ui->listWidgetFont->count() - 1)->setSelected(true);
			ui->listWidgetFont->setCurrentRow(ui->listWidgetFont->count() - 1);
		}
	}

	// Initializing the synchronisation tab

	switch(_settings->synchroProtocol()) {
	case PhSynchronizer::NoSync:
		ui->noSyncRadioButton->setChecked(true);
		break;
	case PhSynchronizer::Sony:
		ui->sonyRadioButton->setChecked(true);
		break;
	case PhSynchronizer::LTC:
		ui->ltcRadioButton->setChecked(true);
		break;
	case PhSynchronizer::MTC:
		ui->mtcRadioButton->setChecked(true);
		break;
	}

	_protocolButtonGroup.addButton(ui->noSyncRadioButton);
	_protocolButtonGroup.addButton(ui->sonyRadioButton);
	_protocolButtonGroup.addButton(ui->ltcRadioButton);
	_protocolButtonGroup.addButton(ui->mtcRadioButton);

	connect(&_protocolButtonGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(updateSynchronisationEnabledControl(int, bool)));
	connect(ui->mmcCheckBox, SIGNAL(toggled(bool)), this, SLOT(updateSynchronisationEnabledControl(bool)));

#ifdef USE_SONY
	// Initializing the sony preferences
	ui->sonyCommunicationTimeCodeTypeComboBox->setCurrentIndex(_settings->sonySlaveCommunicationTimeCodeType());
	ui->sonyVideoSyncTimeCodeTypeComboBox->setCurrentIndex(_settings->sonySlaveVideoSyncTimeCodeType());
#else
	ui->sonyRadioButton->setEnabled(false);
#endif

#ifdef USE_LTC
	// Initializing the LTC preferences
	QStringList ltcInputPorts = PhLtcReader::inputList();
	ui->ltcInputPortComboBox->addItems(ltcInputPorts);
	if(ltcInputPorts.contains(_settings->ltcInputPort()))
		ui->ltcInputPortComboBox->setCurrentText(_settings->ltcInputPort());
#else
	ui->ltcRadioButton->setEnabled(false);
#endif

#ifdef USE_MIDI
	// Initializing MTC preferences
	if (PhMidiObject::canUseVirtualPorts()) {
		ui->mtcVirtualInputPortLineEdit->setText(_settings->mtcVirtualInputPort());
	}
	else {
		ui->mtcVirtualInputPortLineEdit->setEnabled(false);
		ui->mtcVirtualInputPortRadioButton->setEnabled(false);
	}

	QStringList mtcInputPorts = PhMidiInput::inputList();
	ui->mtcExistingInputPortComboBox->addItems(mtcInputPorts);

	if(mtcInputPorts.contains(_settings->mtcInputPort()))
		ui->mtcExistingInputPortComboBox->setCurrentText(_settings->mtcInputPort());

	if(_settings->mtcInputUseExistingPort()) {
		ui->mtcExistingInputPortRadioButton->setChecked(true);
	}
	else {
		ui->mtcVirtualInputPortRadioButton->setChecked(true);
	}

	ui->mtcForce24as2398CheckBox->setChecked(_settings->mtcForce24as2398());

	// Initializing MMC preferences
	ui->mmcCheckBox->setChecked(_settings->sendMmcMessage());
	QStringList mmcOutputPorts = PhMidiOutput::outputList();

	ui->mmcOutputPortComboBox->addItems(mmcOutputPorts);

	if(mmcOutputPorts.contains(_settings->mmcOutputPort()))
		ui->mmcOutputPortComboBox->setCurrentText(_settings->mmcOutputPort());

	_midiPortTypeButtonGroup.addButton(ui->mtcExistingInputPortRadioButton);
	_midiPortTypeButtonGroup.addButton(ui->mtcVirtualInputPortRadioButton);

	connect(&_midiPortTypeButtonGroup, SIGNAL(buttonToggled(int, bool)), this, SLOT(updateSynchronisationEnabledControl(int, bool)));
#else
	ui->mtcRadioButton->setEnabled(false);
	ui->mtcForce24as2398CheckBox->setEnabled(false);
	ui->mmcCheckBox->setEnabled(false);
#endif

	updateSynchronisationEnabledControl(0, false);

	//Initializing the language preferences
	QDir appDirectory(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/");

	QStringList filtersLang;
	filtersLang.append("*.qm");
	appDirectory.setNameFilters(filtersLang);
	QStringList languageFileList = appDirectory.entryList();


	_langNameMap[""] = tr("<System default>");
	ui->cboBoxLang->addItem(_langNameMap[""], "");

	foreach(QString tradFile, languageFileList) {
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

	ui->tabWidget->setCurrentIndex(_settings->lastPreferencesTab());
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::accept()
{
	if(ui->cboBoxLang->currentData() != _settings->language()) {
		QMessageBox::warning(this, tr("Information"),
		                     tr("You change the language to \"%1\".\n"
		                        "You need to restart %2 to apply you changes.")
		                     .arg(ui->cboBoxLang->currentText())
		                     .arg(PH_APP_NAME),
		                     QMessageBox::Ok,
		                     QMessageBox::Ok);
		_settings->setLanguage(ui->cboBoxLang->currentData().toString());
	}

	if(ui->noSyncRadioButton->isChecked())
		_settings->setSynchroProtocol(PhSynchronizer::NoSync);
	else if(ui->sonyRadioButton->isChecked())
		_settings->setSynchroProtocol(PhSynchronizer::Sony);
	else if(ui->ltcRadioButton->isChecked())
		_settings->setSynchroProtocol(PhSynchronizer::LTC);
	else if(ui->mtcRadioButton->isChecked())
		_settings->setSynchroProtocol(PhSynchronizer::MTC);

#ifdef USE_SONY
	_settings->setSonySlaveCommunicationTimeCodeType(ui->sonyCommunicationTimeCodeTypeComboBox->currentIndex());
	_settings->setSonySlaveVideoSyncTimeCodeType(ui->sonyVideoSyncTimeCodeTypeComboBox->currentIndex());
#endif
#ifdef USE_LTC
	_settings->setLtcInputPort(ui->ltcInputPortComboBox->currentText());
#endif

#ifdef USE_MIDI
	_settings->setMtcInputUseExistingPort(ui->mtcExistingInputPortRadioButton->isChecked());
	_settings->setMtcInputPort(ui->mtcExistingInputPortComboBox->currentText());
	_settings->setMtcVirtualInputPort(ui->mtcVirtualInputPortLineEdit->text());
	_settings->setMtcForce24as2398(ui->mtcForce24as2398CheckBox->isChecked());

	_settings->setSendMmcMessage(ui->mmcCheckBox->isChecked());
	_settings->setMmcOutputPort(ui->mmcOutputPortComboBox->currentText());
#endif

	_settings->setLastPreferencesTab(ui->tabWidget->currentIndex());

	QDialog::accept();
}

void PreferencesDialog::reject()
{
	_settings->setScreenDelay(_oldScreenDelay);
	_settings->setVideoSecondScreenDelay(_oldSecondScreenDelay);
	_settings->setVideoPictureInPictureOffset(_oldPipOffset);
	_settings->setVideoPictureInPictureRatio(_oldPipRatio);
	_settings->setVideoPictureInPicturePositionRight(_oldPipPositionRight);
	_settings->setStripHeight(_oldStripHeight);
	_settings->setHorizontalTimePerPixel(_oldHorizontalTimePerPixel);
	_settings->setTextBoldness(_oldBolness);
	_settings->setTextFontFile(_oldFont);

	QDialog::reject();
}

void PreferencesDialog::updateSynchronisationEnabledControl(bool)
{
	PHDEBUG;

	ui->sonyFrame->setEnabled(ui->sonyRadioButton->isChecked());
	ui->ltcFrame->setEnabled(ui->ltcRadioButton->isChecked());
	ui->mtcFrame->setEnabled(ui->mtcRadioButton->isChecked());
	ui->mmcFrame->setEnabled(ui->mmcCheckBox->isChecked());

#ifdef USE_MIDI
	bool useExistingPort = ui->mtcExistingInputPortRadioButton->isChecked();

	_settings->setMtcInputUseExistingPort(useExistingPort);
	_settings->setMtcInputPort(ui->mtcExistingInputPortComboBox->currentText());
	_settings->setMtcVirtualInputPort(ui->mtcVirtualInputPortLineEdit->text());

	ui->mtcVirtualInputPortLineEdit->setEnabled(!useExistingPort);
	ui->mtcExistingInputPortComboBox->setEnabled(useExistingPort);
#endif // USE_MIDI
}

void PreferencesDialog::updateSynchronisationEnabledControl(int, bool)
{
	updateSynchronisationEnabledControl(false);
}

void PreferencesDialog::on_spinBoxSpeed_valueChanged(int speed)
{
	_settings->setHorizontalTimePerPixel(speed);
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
	foreach(QString fontName, _fontList.keys()) {
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

void PreferencesDialog::on_mainScreenDelayspinBox_valueChanged(int delay)
{
	_settings->setScreenDelay(delay);
}

void PreferencesDialog::on_secondScreenDelaySpinBox_valueChanged(int delay)
{
	_settings->setVideoSecondScreenDelay(delay);
}

void PreferencesDialog::on_pipOffsetSpinBox_valueChanged(int offset)
{
	_settings->setVideoPictureInPictureOffset(offset);
}

void PreferencesDialog::on_pipRatioSlider_valueChanged(int value)
{
	_settings->setVideoPictureInPictureRatio(value / 100.0f);
}

void PreferencesDialog::on_pipRightPositionRadioButton_toggled(bool checked)
{
	_settings->setVideoPictureInPicturePositionRight(checked);
}
