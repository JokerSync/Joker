/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

#include "PhAudio/PhAudioOutput.h"
#include "PhAudio/PhAudioInput.h"

PreferencesDialog::PreferencesDialog(QString audioOutput, QString audioInput, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesDialog)
{
	ui->setupUi(this);
	QList<QString> outputList = PhAudioOutput::outputList();
	ui->comboBoxOutput->addItems(outputList);
	if(outputList.contains(audioOutput))
		ui->comboBoxOutput->setCurrentText(audioOutput);

	QList<QString> inputList = PhAudioInput::inputList();
	ui->comboBoxInput->addItems(inputList);
	if(inputList.contains(audioInput))
		ui->comboBoxInput->setCurrentText(audioInput);
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

QString PreferencesDialog::selectedAudioOutput()
{
	return ui->comboBoxOutput->currentText();
}

QString PreferencesDialog::selectedAudioInput()
{
	return ui->comboBoxInput->currentText();
}

