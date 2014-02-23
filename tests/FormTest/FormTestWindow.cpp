#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QPainter>
#include <QMessageBox>
#include <QScreen>
#include <QTime>

#include "FormTestWindow.h"
#include "ui_FormTestWindow.h"

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"

FormTestWindow::FormTestWindow(FormTestSettings *settings, QWidget *parent) :
	PhDocumentWindow(settings, parent),
	ui(new Ui::FormTestWindow),
	_settings(settings),
	_image(NULL),
	_rgb(NULL)
{
	ui->setupUi(this);

	QString mode = _settings->currentDocument();
	if(mode == "rgb")
		generateRGB();
	else if(mode == "yuv")
		generateYUV();

	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen)
		ui->labelFrequency->setText(QString::number(screen->refreshRate()) + "hz");
}

FormTestWindow::~FormTestWindow()
{
	delete ui;
}

void FormTestWindow::generateRGB()
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

void FormTestWindow::generateYUV()
{
	if(_rgb)
		delete _rgb;
	if(_image)
		delete _image;
	int w = 300;
	int h = 200;
	unsigned char *yuv = PhPictureTools::generateYUVPattern(w, h);
	_rgb = new unsigned char[w * h * 3];
	QTime timer;
	timer.start();
	for(int i = 0; i < 10; i++)
		PhPictureTools::ConvertYV12toRGB(yuv, _rgb, w, h);
	PHDEBUG << "elapsed: " << timer.elapsed();
	_image = new QImage(_rgb, w, h, QImage::Format_RGB888);
	this->update();
	delete yuv;
}

bool FormTestWindow::openDocument(QString fileName)
{
	if(_image)
		delete _image;
	_image = new QImage();
	if(_image->load(fileName)) {
		this->update();
		setCurrentDocument(fileName);
		return true;
	}
	else {
		delete _image;
		return false;
	}
}

void FormTestWindow::paintEvent(QPaintEvent *)
{
	if(_image) {
		QPainter painter(this);
		painter.drawImage(0, 0, *_image);
	}
}

QMenu *FormTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

void FormTestWindow::on_actionAbout_triggered()
{
	_about.exec();
}

void FormTestWindow::on_actionDocumentation_triggered()
{
	if(QDesktopServices::openUrl(QUrl("http://www.doublage.org", QUrl::TolerantMode)))
		PHDEBUG <<"openned url correctly";
}

void FormTestWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open a picture file...", _settings->lastDocumentFolder(), "Picture file (*.png *.jpg)");
	if(QFile::exists(fileName)) {
		if(!openDocument(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void FormTestWindow::on_actionGenerate_YUV_pattern_triggered()
{
	generateYUV();
	_settings->setCurrentDocument("yuv");
}

void FormTestWindow::on_actionGenerate_RGB_pattern_triggered()
{
	generateRGB();
	_settings->setCurrentDocument("rgb");
}
