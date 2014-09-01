/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhMidi/PhMidiOutput.h"

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(MidiToolSettings *settings) :
	ui(new Ui::PreferencesDialog),
	_settings(settings),
	_oldMidiInputPortName(settings->midiInputPortName()),
	_oldMidiOutputPortName(settings->midiOutputPortName())
{
	ui->setupUi(this);

	QStringList outputList = PhMidiOutput::outputList();

	// We add manually the output since it has been closed
	outputList.append(settings->midiInputPortName());
	ui->comboBoxOutput->addItems(outputList);
	if(outputList.contains(settings->midiOutputPortName()))
		ui->comboBoxOutput->setCurrentText(settings->midiOutputPortName());

	ui->lineEditInput->setText(settings->midiInputPortName());
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::accept()
{
	_settings->setMidiInputPortName(ui->lineEditInput->text());
	_settings->setMidiOutputPortName(ui->comboBoxOutput->currentText());

	QDialog::accept();
}

void PreferencesDialog::reject()
{
	_settings->setMidiInputPortName(_oldMidiInputPortName);
	_settings->setMidiOutputPortName(_oldMidiOutputPortName);

	QDialog::reject();
}

