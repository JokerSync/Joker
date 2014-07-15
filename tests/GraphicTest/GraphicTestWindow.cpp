#include <QFileDialog>
#include <QMessageBox>

#include "GraphicTestWindow.h"
#include "ui_GraphicTestWindow.h"

#include "PhTools/PhPictureTools.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicLoop.h"
#include "PhGraphic/PhGraphicDisc.h"
#include "PhGraphic/PhGraphicDashedLine.h"
#include "PhGraphic/PhGraphicArrow.h"

GraphicTestWindow::GraphicTestWindow(GraphicTestSettings *settings) :
	PhWindow(settings),
	ui(new Ui::GraphicTestWindow),
	_settings(settings)
{
	ui->setupUi(this);
	ui->graphicView->setGraphicSettings(_settings);

	connect(ui->graphicView, &PhGraphicView::paint, this, &GraphicTestWindow::onPaint);

	PHDEBUG << "Initialize _image";

	QString imageFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240.png";
	_image.setFilename(imageFile);
	_image.setTextureCoordinate(1, 1);
	_image.setPosition(50, 0, 1);

	PHDEBUG << "Initialize _font";

	_font1.setFontFile(_settings->font1File());
	_font2.setFontFile(_settings->font2File());

	PHDEBUG << "Initialize _rect";
	_rect.setRect(100, 100, 75, 40);
	_rect.setColor(QColor(0, 255, 0));
	_rect.setZ(-2);

	int w = 64;
	int h = 64;
	unsigned char * yuv = PhPictureTools::generateYUVPattern(w, h);
	_yuvRect.setRect(20, 300, 64, 64);
	_yuvRect.createTextureFromYUVBuffer(yuv, w, h);

	unsigned char * rgb = PhPictureTools::generateRGBPattern(w, h);
	_rgbRect.setRect(200, 300, 64, 64);
	_rgbRect.createTextureFromRGBBuffer(rgb, w, h);
}

GraphicTestWindow::~GraphicTestWindow()
{
	delete ui;
}


void GraphicTestWindow::on_actionChange_font_triggered()
{
	QString fileName = QFileDialog::getOpenFileName();
	if(QFile(fileName).exists()) {
		if(PhFont::computeMaxFontSize(fileName) == 0)
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
		else
			_font1.setFontFile(fileName);
	}
}

void GraphicTestWindow::on_actionSave_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save...");
	QImage image = ui->graphicView->grabFrameBuffer();
	if(!image.save(fileName))
		QMessageBox::critical(this, "Error", QString("Unable to save %1").arg(fileName));
}

void GraphicTestWindow::onPaint(int width, int height)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	_image.setTextureCoordinate(1, 3);
	_image.setSize(_image.originalSize());
	_image.draw();

	_rect.setColor(QColor(0, 255, 0));
	_rect.setRect(50, 100, 500, 75);
	_rect.draw();

	_rect.setColor(QColor(0, 255, 255));
	_rect.setRect(50, 175, 500, 25);
	_rect.draw();

	int textCount = 1;
	int quadCount = 1;
	QString textContent("Change the text from the settings");

	if(_settings) {
		quadCount = _settings->quadCount();
		textCount = _settings->textCount();
		textContent = _settings->textContent();
	}

	for(int i = 0; i < textCount; i++) {
		PhGraphicText text1(&_font1, textContent);

		text1.setRect(i % 200, i / 200, 500, 100);
		text1.setColor(QColor(128, 255, 0));
		text1.setZ(5);
		text1.draw();
	}

	_font1.setFontFile(_settings->font1File());
	_font1.select();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glColor3f(0, 0, 1);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);


	glBegin(GL_QUADS);  //Begining the cube's drawing
	{
		for(int i = 0; i < quadCount; i++) {
			glTexCoord3f(0, 0, 1);  glVertex3i(0, 0, 0);
			glTexCoord3f(1, 0, 1);  glVertex3i(width, 0, 0);
			glTexCoord3f(1, 1, 1);  glVertex3i(width, height, 0);
			glTexCoord3f(0, 1, 1);  glVertex3i(0,  height, 0);
		}
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	//	_text.setX(_text.getX() + 4);
	//	if(_text.getX() > this.width())
	//		_text.setX(0);
	//	if((_text.getX()+_text.getWidth()) < 0)
	//		_text.setX(this.width());

	PhGraphicSolidRect rect3(0, 800, 400, 300);
	rect3.setColor(Qt::blue);
	rect3.draw();

	PhGraphicLoop vLoop;
	vLoop.setPosition(100, 500, 3);
	vLoop.setSize(220, 200);
	vLoop.setThickness(30);
	vLoop.setCrossSize(100);
	vLoop.setColor(QColor(255, 0, 255));
	vLoop.draw();

	PhGraphicLoop hLoop;
	hLoop.setHorizontalLoop(true);
	hLoop.setPosition(0, 800, 3);
	hLoop.setSize(300, 50);
	hLoop.setThickness(20);
	hLoop.setCrossSize(60);
	hLoop.setColor(QColor(255, 0, 255));
	hLoop.draw();

	//_yuvRect.draw();

	PhGraphicDisc disc(300, 300, 100);
	disc.setColor(Qt::yellow);
	disc.draw();

	for (int i = 0; i < 5; ++i) {
		PhGraphicDashedLine line(i, 0, 50*i, 300, 30);
		line.setColor(Qt::green);
		line.setZ(4);
		line.draw();
	}

	PhGraphicArrow arrow1(PhGraphicArrow::DownLeftToUpRight, 150, 250, 200, 100);
	arrow1.setColor(Qt::red);
	arrow1.setZ(5);
	arrow1.draw();

	PhGraphicArrow arrow2(PhGraphicArrow::UpLefToDownRight, 150, 400, 200, 100);
	arrow2.setColor(Qt::red);
	arrow2.setZ(5);
	arrow2.draw();

	_yuvRect.draw();
	_rgbRect.draw();

	_font2.setFontFile(_settings->font2File());
	_font2.select();

	PhGraphicText text2(&_font2, "eéaàiîoô");
	int textWidth = 500;
	text2.setRect(_x, 300, textWidth, 100);
	text2.setColor(QColor(255, 0, 0));
	text2.setZ(1);
	text2.draw();

	_x += 4;
	if(_x > width)
		_x = -textWidth;
}
