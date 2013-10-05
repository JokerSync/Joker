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

	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(accept()));

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

void PhTimeCodeDialog::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	if(ui->_timecodeEdit->isTimeCode())
		ui->okButton->setEnabled(true);
	else
		ui->okButton->setEnabled(false);
}
