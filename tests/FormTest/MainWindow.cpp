#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QMessageBox>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "PhTools/PhDebug.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	_settings("Phonations","FormTest")
{
    ui->setupUi(this);
	ui->graphicsView->setScene(&scene);

	openFile(_settings.value("lastFile").toString());
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::openFile(QString fileName)
{
	QPixmap * pixmap = new QPixmap(fileName);
	if(pixmap->isNull())
		return false;
	QGraphicsPixmapItem * item = new QGraphicsPixmapItem(*pixmap);

	scene.clear();
	scene.addItem((item));
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
		else
			QMessageBox::critical(this, "Error", "Unable to open file");
	}
}
