#include "StripPropertiesDialog.h"
#include "ui_StripPropertiesDialog.h"

StripPropertiesDialog::StripPropertiesDialog(QString title, QString path, QString ips, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::StripPropertiesDialog)
{
	ui->setupUi(this);
	ui->Title->setText(title);
	ui->Path->setText(path);
	ui->Ips->setText(ips);
}

StripPropertiesDialog::~StripPropertiesDialog()
{
	delete ui;
}
