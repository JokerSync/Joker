#include "ChangeTextDialog.h"
#include "ui_ChangeTextDialog.h"

ChangeTextDialog::ChangeTextDialog(QString text, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ChangeTextDialog)
{
	ui->setupUi(this);
	ui->lineEdit->setText(text);
}

ChangeTextDialog::~ChangeTextDialog()
{
	delete ui;
}

void ChangeTextDialog::on_lineEdit_textChanged(const QString &text)
{
	emit textChanged(text);
}
