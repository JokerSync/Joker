#include "VideoPropertiesDialog.h"
#include "ui_VideoPropertiesDialog.h"
#include <QFileInfo>
#include "PhTools/PhStringTools.h"

VideoPropertiesDialog::VideoPropertiesDialog(QString path, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::VideoPropertiesDialog)
{
	ui->setupUi(this);

	QFileInfo fileInfo(path);
	QString title = fileInfo.fileName();
	int size = fileInfo.size();
	QString videoSize = PhStringTools::sizeToString(size);

	ui->titleContentLabel->setText(title);
	ui->pathContentLabel->setText(path);
	ui->sizeContentLabel->setText(videoSize);
}

VideoPropertiesDialog::~VideoPropertiesDialog()
{
	delete ui;
}
