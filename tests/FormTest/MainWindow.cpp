#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QMessageBox>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	_settings("Phonations","FormTest")
{
    ui->setupUi(this);
	ui->graphicsView->setScene(&scene);

	QString mode = _settings.value("mode", "rgb").toString();
	if(mode == "rgb")
		generateRGB();
	else if(mode == "yuv")
		generateYUV();
	else
		openFile(mode);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::generateRGB()
{
	unsigned char *rgb = PhPictureTools::generateRGBPattern(100, 100);

}

void MainWindow::generateYUV()
{
	unsigned char *yuv = PhPictureTools::generateYUVPattern(100, 100);

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
			_settings.setValue("mode", fileName);
		else
			QMessageBox::critical(this, "Error", "Unable to open file");
	}
}

void MainWindow::on_actionGenerate_YUV_pattern_triggered()
{
    generateYUV();
	_settings.setValue("mode", "yuv");
}

void MainWindow::on_actionGenerate_RGB_pattern_triggered()
{
	generateRGB();
	_settings.setValue("mode", "rgb");
}
