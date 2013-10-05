#include <QDesktopServices>
#include <QUrl>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "PhTools/PhDebug.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _frame = 5882;
    _tcType = PhTimeCodeType25;
	ui->_timeCodeLabel->setText(PhTimeCode::stringFromFrame(_frame, _tcType));

	loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionGoto_triggered()
{
	PhTimeCodeDialog dlg(_tcType, _frame);
	if(dlg.exec() == QDialog::Accepted)
	{
		_frame = dlg.frame();
		ui->_timeCodeLabel->setText(PhTimeCode::stringFromFrame(_frame, _tcType));
	}
}

void MainWindow::on_actionAbout_triggered()
{
	_about->exec();
}

void MainWindow::on_actionDocumentation_triggered()
{
	if(QDesktopServices::openUrl(QUrl("http://www.doublage.org",QUrl::TolerantMode)))
		PHDEBUG <<"openned url correctly";
}

void MainWindow::on__saveButton_clicked()
{
	saveSettings();
}

void MainWindow::on__loadButton_clicked()
{
	loadSettings();
}

void MainWindow::saveSettings()
{
	QSettings setting("Phonations","FormTest");
	setting.setValue("qqwweerrttyy",ui->_lineEdit->text());

	PHDEBUG << "Settings saved";
}

void MainWindow::loadSettings()
{
	QSettings setting("Phonations","FormTest");
	ui->_lineEdit->setText(setting.value("qqwweerrttyy", "julien").toString());

	PHDEBUG << "Settings loaded";
}
