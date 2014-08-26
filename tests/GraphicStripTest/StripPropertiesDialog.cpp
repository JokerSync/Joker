#include "StripPropertiesDialog.h"
#include "ui_StripPropertiesDialog.h"

StripPropertiesDialog::StripPropertiesDialog(PhStripDoc * doc, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::StripPropertiesDialog)
{
	ui->setupUi(this);

	QString title = doc->title();

	PhFrame Fps = PhTimeCode::getFps(doc->videoTimeCodeType());
	QString ips = QString::number(Fps);

	QString filePath = doc->filePath();

	QString videoPath = doc->videoFilePath();

	ui->titleContentLabel->setText(title);
	ui->videoPathContentLabel->setText(videoPath);
	ui->pathContentLabel->setText(filePath);
	ui->fpsContentLabel->setText(ips);
}

StripPropertiesDialog::~StripPropertiesDialog()
{
	delete ui;
}
