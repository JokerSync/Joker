/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhMidi/PhMidiOutput.h"
#include "PhMidi/PhMidiInput.h"

#include "PreferencesDialog.h"
#include "ui_PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(MidiToolSettings *settings) :
	ui(new Ui::PreferencesDialog),
	_settings(settings),
	_oldMidiOutputPortName(settings->midiOutputPortName()),
	_oldMidiInputPortName(settings->midiInputPortName()),
	_oldMidiVirtualInputPortName(settings->midiVirtualInputPortName())
{
	ui->setupUi(this);

	QStringList outputList = PhMidiOutput::outputList();

	// We add manually the output since it has been closed
	outputList.append(_settings->midiInputPortName());
	ui->comboBoxOutput->addItems(outputList);

	if(outputList.contains(_settings->midiOutputPortName()))
		ui->comboBoxOutput->setCurrentText(_settings->midiOutputPortName());

	ui->lineEditInput->setText(_settings->midiVirtualInputPortName());

	QStringList inputList = PhMidiInput::inputList();
	ui->comboBoxInput->addItems(inputList);

	bool useExistingPort = inputList.contains(_settings->midiInputPortName());
	if(useExistingPort) {
		ui->comboBoxInput->setCurrentText(_settings->midiInputPortName());
		ui->radioButtonExistingPort->setChecked(true);
	}
	else {
		_settings->setMidiVirtualInputPortName(_settings->midiInputPortName());
		ui->radioButtonVirtualPort->setChecked(true);
	}

	updateInputPortEnabledControl();

	connect(ui->radioButtonExistingPort, &QRadioButton::toggled, this, &PreferencesDialog::updateInputPortEnabledControl);
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::updateInputPortEnabledControl()
{
	PHDEBUG;
	bool useExistingPort = ui->radioButtonExistingPort->isChecked();
	if(useExistingPort)
		_settings->setMidiInputPortName(ui->comboBoxInput->currentText());
	else
		_settings->setMidiInputPortName(ui->lineEditInput->text());

	ui->comboBoxInput->setEnabled(useExistingPort);
	ui->lineEditInput->setEnabled(!useExistingPort);
}

void PreferencesDialog::accept()
{
	_settings->setMidiOutputPortName(ui->comboBoxOutput->currentText());
	if(ui->radioButtonExistingPort->isChecked())
		_settings->setMidiInputPortName(ui->comboBoxInput->currentText());
	else
		_settings->setMidiInputPortName(ui->lineEditInput->text());
	_settings->setMidiVirtualInputPortName(ui->lineEditInput->text());

	QDialog::accept();
}

void PreferencesDialog::reject()
{
	_settings->setMidiInputPortName(_oldMidiInputPortName);
	_settings->setMidiOutputPortName(_oldMidiOutputPortName);
	_settings->setMidiVirtualInputPortName(_oldMidiVirtualInputPortName);

	QDialog::reject();
}

