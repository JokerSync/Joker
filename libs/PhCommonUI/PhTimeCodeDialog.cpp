/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTimeCodeDialog.h"
#include "ui_PhTimeCodeDialog.h"
#include <QDialogButtonBox>
#include <QPushButton>

PhTimeCodeDialog::PhTimeCodeDialog(PhTimeCodeType tcType, PhTime time, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhTimeCodeDialog)
{
	ui->setupUi(this);
	ui->_timecodeEdit->setTime(time, tcType);

	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));

	ui->okButton->setDefault(true);
}

PhTimeCodeDialog::~PhTimeCodeDialog()
{
	delete ui;
}

PhTime PhTimeCodeDialog::time()
{
	return ui->_timecodeEdit->time();
}

void PhTimeCodeDialog::onFrameChanged(PhFrame, PhTimeCodeType)
{
	if(ui->_timecodeEdit->isTimeCode())
		ui->okButton->setEnabled(true);
	else
		ui->okButton->setEnabled(false);
}
