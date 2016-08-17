/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "AudioTestWindow.h"
#include "ui_AudioTestWindow.h"
#include "PreferencesDialog.h"

AudioTestWindow::AudioTestWindow(QSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	_settings(settings),
	ui(new Ui::AudioTestWindow)
{
	ui->setupUi(this);

	on_generateCheckBox_clicked(_settings->value("generate", true).toBool());
	on_readCheckBox_clicked(_settings->value("read", true).toBool());

	connect(&_audioReader, SIGNAL(audioProcessed(int,int)), this, SLOT(onAudioProcessed(int,int)));
}

AudioTestWindow::~AudioTestWindow()
{
	delete ui;
}

void AudioTestWindow::on_actionPreferences_triggered()
{
	PreferencesDialog dlg(_settings->value("audioOutput").toString(), _settings->value("audioInput").toString());
	if(dlg.exec()) {
		PHDEBUG << dlg.selectedAudioOutput();
		_settings->setValue("audioOutput", dlg.selectedAudioOutput());
		_settings->setValue("audioInput", dlg.selectedAudioInput());

		if(_settings->value("generate", true).toBool())
			setupOutput();
		if(_settings->value("read", true).toBool())
			setupInput();
	}
}

void AudioTestWindow::setupOutput()
{
	_audioWriter.close();
	if(!_audioWriter.init(_settings->value("audioOutput", "").toString())) {
		QMessageBox::warning(this, tr("Error"),
		                     tr("Error while loading the output device.\n"
		                        "See log for more informations"),
		                     QMessageBox::Ok);
		on_generateCheckBox_clicked(false);
	}
}

void AudioTestWindow::setupInput()
{
	_audioReader.close();
	if(!_audioReader.init(_settings->value("audioInput", "").toString())) {
		QMessageBox::warning(this, tr("Error"),
		                     tr("Error while loading the input device.\n"
		                        "See log for more informations"),
		                     QMessageBox::Ok);
		on_readCheckBox_clicked(false);
	}
}

void AudioTestWindow::on_generateCheckBox_clicked(bool checked)
{
	ui->generatorGroupBox->setEnabled(checked);
	ui->generateCheckBox->setChecked(checked);
	_settings->setValue("generate", checked);
	if(checked)
		setupOutput();
	else
		_audioWriter.close();
}

void AudioTestWindow::on_readCheckBox_clicked(bool checked)
{
	ui->readerGroupBox->setEnabled(checked);
	ui->readCheckBox->setChecked(checked);
	_settings->setValue("read", checked);
	if(checked)
		setupInput();
	else
		_audioReader.close();
}

void AudioTestWindow::onAudioProcessed(int minLevel, int maxLevel)
{
	ui->inputLevelLabel->setText(QString("Input level : %1 / %2").arg(minLevel).arg(maxLevel));
}
