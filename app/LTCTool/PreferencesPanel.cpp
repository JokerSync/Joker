/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PreferencesPanel.h"
#include "ui_PreferencesPanel.h"

#include "PhSync/PhLtcWriter.h"
#include "PhSync/PhLtcReader.h"

PreferencesPanel::PreferencesPanel(QString audioOutput, QString audioInput, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesPanel)
{
	ui->setupUi(this);
	QList<QString> outputList = PhLtcWriter::outputList();
	ui->comboBoxOutput->addItems(outputList);
	if(outputList.contains(audioOutput))
		ui->comboBoxOutput->setCurrentText(audioOutput);

	QList<QString> inputList = PhLtcReader::inputList();
	ui->comboBoxInput->addItems(inputList);
	if(inputList.contains(audioInput))
		ui->comboBoxInput->setCurrentText(audioInput);
}

PreferencesPanel::~PreferencesPanel()
{
	delete ui;
}

QString PreferencesPanel::selectedAudioOutput()
{
	return ui->comboBoxOutput->currentText();
}

QString PreferencesPanel::selectedAudioInput()
{
	return ui->comboBoxInput->currentText();
}

