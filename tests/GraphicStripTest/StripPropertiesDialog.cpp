#include "StripPropertiesDialog.h"
#include "ui_StripPropertiesDialog.h"

StripPropertiesDialog::StripPropertiesDialog(PhStripDoc * doc, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::StripPropertiesDialog)
{
	ui->setupUi(this);

	QString title = doc->getTitle();

	PhFrame Fps = PhTimeCode::getFps(doc->getTCType());
	QString ips = QString::number(Fps);

	QString filePath = doc->getFilePath();

	QString videoPath = doc->getVideoPath();

	ui->titleContentLabel->setText(title);
	ui->videoPathContentLabel->setText(videoPath);
	ui->pathContentLabel->setText(filePath);
	ui->fpsContentLabel->setText(ips);
}

StripPropertiesDialog::~StripPropertiesDialog()
{
	delete ui;
}
