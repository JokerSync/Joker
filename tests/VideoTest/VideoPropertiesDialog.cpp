#include "VideoPropertiesDialog.h"
#include "ui_VideoPropertiesDialog.h"

VideoPropertiesDialog::VideoPropertiesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::VideoPropertiesDialog)
{
	ui->setupUi(this);
}

VideoPropertiesDialog::~VideoPropertiesDialog()
{
	delete ui;
}
