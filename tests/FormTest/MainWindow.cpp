#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "PhTools/PhDebug.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	_settings("Phonations","FormTest")
{
    ui->setupUi(this);

	openFile(_settings.value("lastFile").toString());
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::openFile(QString fileName)
{
	ui->_lineEdit->setText(fileName);
	return true;
}

void MainWindow::on_actionAbout_triggered()
{
	_about.exec();
}

void MainWindow::on_actionDocumentation_triggered()
{
	if(QDesktopServices::openUrl(QUrl("http://www.doublage.org",QUrl::TolerantMode)))
		PHDEBUG <<"openned url correctly";
}

void MainWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this);
	if(QFile::exists(fileName))
	{
		if(openFile(fileName))
			_settings.setValue("lastFile", fileName);
	}
}
