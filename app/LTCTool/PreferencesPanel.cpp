#include "PreferencesPanel.h"
#include "ui_PreferencesPanel.h"

#include "PhLtcWriter.h"

PreferencesPanel::PreferencesPanel(QString audioOutput, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PreferencesPanel)
{
	ui->setupUi(this);
	foreach(QString output, PhLtcWriter::outputList())
	{
		ui->comboBoxOutput->addItem(output);
		if(output == audioOutput)
		{
			PHDEBUG << "Match";
			ui->comboBoxOutput->setCurrentIndex(ui->comboBoxOutput->children().length());
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

