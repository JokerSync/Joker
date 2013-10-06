#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
	_settings("Phonations","FormTest"),
	_image(NULL),
	_rgb(NULL)
{
    ui->setupUi(this);

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
	if(_rgb)
		delete _rgb;
	if(_image)
		delete _image;
	int w = 100;
	int h = 100;
	_rgb = PhPictureTools::generateRGBPattern(w, h);
	_image = new QImage(_rgb, w, h, QImage::Format_RGB888);
	this->update();
}

void MainWindow::generateYUV()
{
	if(_rgb)
		delete _rgb;
	if(_image)
		delete _image;
	int w = 100;
	int h = 100;
	unsigned char *yuv = PhPictureTools::generateYUVPattern(w, h);
	_rgb = new unsigned char[w * h * 3];
	PhPictureTools::ConvertYV12toRGB(yuv, _rgb, w, h);
	_image = new QImage(_rgb, w, h, QImage::Format_RGB888);
	this->update();
	delete yuv;
}

bool MainWindow::openFile(QString fileName)
{
	if(_image)
		delete _image;
	_image = new QImage();
	if(_image->load(fileName))
	{
		this->update();
		return true;
	}
	else
	{
		delete _image;
		return false;
	}
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	if(_image)
	{
		QPainter painter(this);
		painter.drawImage(0, 0, *_image);
	}
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
