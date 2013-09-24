#include <QDesktopServices>
#include <QUrl>

#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _frame = 5882;
    _tcType = PhTimeCodeType25;
	_goto = new PhTimeCodeDialog(this);
	_about = new AboutMenu(this);
	ui->_timeCodeLabel->setText(PhTimeCode::stringFromFrame(_frame, _tcType));

    connect(_goto, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(updateText(PhFrame, PhTimeCodeType)));
	loadSettings();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateText(PhFrame frame, PhTimeCodeType tcType)
{
	ui->_timeCodeLabel->setText(PhTimeCode::stringFromFrame(frame, tcType));
}

void MainWindow::on_actionGoto_triggered()
{
	_goto->setFrame(ui->_timeCodeLabel->text());
    _goto->show();
}

void MainWindow::on_actionAbout_triggered()
{
	_about->exec();
}

void MainWindow::on_actionDocumentation_triggered()
{
	if(QDesktopServices::openUrl(QUrl("http://www.doublage.org",QUrl::TolerantMode)))
		qDebug()<<"openned url correctly";
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

	qDebug() << "Settings saved";
}

void MainWindow::loadSettings()
{
	QSettings setting("Phonations","FormTest");
	ui->_lineEdit->setText(setting.value("qqwweerrttyy", "julien").toString());

	qDebug() << "Settings loaded";
}
