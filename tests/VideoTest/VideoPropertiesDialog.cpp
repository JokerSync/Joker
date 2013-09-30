#include "VideoPropertiesDialog.h"
#include "ui_VideoPropertiesDialog.h"
#include <QFileInfo>
#include "PhTools/PhStringTools.h"

VideoPropertiesDialog::VideoPropertiesDialog(QString path, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::VideoPropertiesDialog)
{
	ui->setupUi(this);
	ui->Title->setText("Title : ");
	ui->Path->setText("path : ");
	ui->Size->setText("size : ");

	QFileInfo fileInfo(path);
	QString title = fileInfo.fileName();
	int size = fileInfo.size();
	QString videoSize = PhStringTools::sizeToString(size);

	ui->TitleContent->setText(title);
	ui->PathContent->setText(path);
	ui->SizeContent->setText(videoSize);
}

VideoPropertiesDialog::~VideoPropertiesDialog()
{
	delete ui;
}
