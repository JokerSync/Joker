/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTimeCodeDialog.h"
#include "ui_PhTimeCodeDialog.h"
#include <QDialogButtonBox>
#include <QPushButton>

PhTimeCodeDialog::PhTimeCodeDialog(PhTimeCodeType tcType, PhFrame frame, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhTimeCodeDialog)
{
	ui->setupUi(this);
	ui->_timecodeEdit->setFrame(frame, tcType);

	connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
	connect(ui->okButton, &QPushButton::clicked, this, &QDialog::accept);

	ui->okButton->setDefault(true);
}

PhTimeCodeDialog::~PhTimeCodeDialog()
{
	delete ui;
}

PhFrame PhTimeCodeDialog::frame()
{
	return ui->_timecodeEdit->frame();
}

PhTime PhTimeCodeDialog::time()
{
	return ui->_timecodeEdit->time();
}

void PhTimeCodeDialog::onTimeChanged(PhTime)
{
	if(ui->_timecodeEdit->isTimeCode())
		ui->okButton->setEnabled(true);
	else
		ui->okButton->setEnabled(false);
}
