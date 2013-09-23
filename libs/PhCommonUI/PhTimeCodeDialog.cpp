#include "PhTimeCodeDialog.h"
#include "ui_PhTimeCodeDialog.h"
#include <QDialogButtonBox>
#include <QPushButton>

PhTimeCodeDialog::PhTimeCodeDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhTimeCodeDialog)
{
	ui->setupUi(this);
	connect(ui->_timecodeEdit, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)));
}

PhTimeCodeDialog::~PhTimeCodeDialog()
{
	delete ui;
}

void PhTimeCodeDialog::setFrame(QString frame)
{
	if(ui->_timecodeEdit->isTimeCode(frame))
	{
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		ui->_timecodeEdit->setText(frame);
	}
	else
	{
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	qDebug() << "setframe";

}
