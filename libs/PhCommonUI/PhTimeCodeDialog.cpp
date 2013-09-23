#include "PhTimeCodeDialog.h"
#include "ui_PhTimeCodeDialog.h"

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
