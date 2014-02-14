#include "PreferencesPanel.h"
#include "ui_PreferencesPanel.h"

#include "PhSync/PhLtcWriter.h"
#include "PhSync/PhLtcReader.h"

PreferencesPanel::PreferencesPanel(QString audioOutput, QString audioInput, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesPanel)
{
	ui->setupUi(this);
	foreach(QString output, PhLtcWriter::outputList())
	{
		ui->comboBoxOutput->addItem(output);
		if(output == audioOutput) {
			ui->comboBoxOutput->setCurrentIndex(ui->comboBoxOutput->children().length());
		}
	}
	foreach(QString input, PhLtcReader::inputList())
	{
		ui->comboBoxInput->addItem(input);
		if(input == audioInput) {
			ui->comboBoxInput->setCurrentIndex(ui->comboBoxInput->children().length());
		}
	}
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

