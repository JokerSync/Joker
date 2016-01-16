#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);
	QString version = QString("%1 v%2").arg(APP_NAME).arg(APP_VERSION);
	if(QString(PH_GIT_BRANCH) != "master")
		version += QString(" %1").arg(PH_GIT_BRANCH);
	ui->versionLabel->setText(version);
	ui->revisionLabel->setText(QString("<a href='https://github.com/phonations/joker/tree/%1'>%2</a>").arg(PH_GIT_REVISION).arg(QString(PH_GIT_REVISION).left(7)));
}

AboutDialog::~AboutDialog()
{
	delete ui;
}
