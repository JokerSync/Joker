#include <QFileDialog>
#include <QMessageBox>
#include <QFontDialog>
#include <QFontDatabase>

#include "GraphicTestWindow.h"
#include "ui_GraphicTestWindow.h"

#include "ChangeTextDialog.h"

#include "PhTools/PhDebug.h"
#include "PhTools/PhPictureTools.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicLoop.h"
#include "PhGraphic/PhGraphicDisc.h"
#include "PhGraphic/PhGraphicDashedLine.h"
#include "PhGraphic/PhGraphicArrow.h"

GraphicTestWindow::GraphicTestWindow(GraphicTestSettings *settings) :
	PhWindow(settings),
	ui(new Ui::GraphicTestWindow),
	_settings(settings),
	_zoom(1),
	_xOffset(0),
	_yOffset(0)
{
	ui->setupUi(this);
	ui->graphicView->setGraphicSettings(_settings);

	connect(ui->graphicView, &PhGraphicView::paint, this, &GraphicTestWindow::onPaint);

	ui->actionInfos->setChecked(_settings->displayInfo());
	ui->actionImage->setChecked(_settings->displayImage());
	ui->actionDisc->setChecked(_settings->displayDisc());
	ui->actionLoops->setChecked(_settings->displayLoops());
	ui->actionArrows->setChecked(_settings->displayArrows());
	ui->actionRectangles->setChecked(_settings->displayRect());
	ui->actionCharacters->setChecked(_settings->displayCharacters());
	ui->actionStatic_text->setChecked(_settings->displayStaticText());
	ui->actionMoving_text->setChecked(_settings->displayMovingText());

	QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/Cappella.ttf");

	PHDEBUG << "Initialize _image";

	QString imageFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/phonations.png";
	_image.setFilename(imageFile);
	_image.setTextureCoordinate(1, 1);
	_image.setPosition(50, 0, 1);

	PHDEBUG << "Initialize _font";

	_font1.setFamily(_settings->font1Family());
	_font2.setFamily(_settings->font2Family());

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
	QFont currentFont(_settings->font1Family());
	QFontDialog dlg(currentFont, this);
	connect(&dlg, &QFontDialog::currentFontChanged, this, &GraphicTestWindow::onFontSelected);
	if(dlg.exec() != QDialog::Accepted) {
		onFontSelected(currentFont);
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
	glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(_settings->displayImage()) {
		_image.setTextureCoordinate(1, 1);
		_image.setSize(_image.originalSize());
		_image.setTransparent(true);
		_image.draw();
	}

	if(_settings->displayRect()) {
		_rect.setColor(QColor(0, 255, 0));
		_rect.setRect(50, 100, 500, 75);
		_rect.draw();

		_rect.setColor(QColor(0, 255, 255));
		_rect.setRect(50, 175, 500, 25);
		_rect.draw();
	}

	if(_settings->displayCharacters()) {
		int w = _zoom * width / 16;
		int h = _zoom * height / 16;
		for(int i = 0; i < 16; i++) {
			for(int j = 0; j < 16; j++) {
				PhGraphicSolidRect rect(i * w, j * h, w, h);
				if ((i % 2) != (j % 2)) {
					rect.setColor(Qt::red);
				}
				else {
					rect.setColor(Qt::blue);
				}
//				rect.setColor(QColor(i * 16, j*16, 255 - i * 8 - j * 8));
				rect.draw();
			}
		}

		_font1.setWeight(_settings->textWeight());
		_font1.select();

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glColor3f(1, 1, 0);

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);  //Begining the cube's drawing
		{
			glTexCoord3f(0, 0, 1);  glVertex3i(_xOffset * w, _yOffset * h, 0);
			glTexCoord3f(1, 0, 1);  glVertex3i(_xOffset * w + _zoom * width, _yOffset * h, 0);
			glTexCoord3f(1, 1, 1);  glVertex3i(_xOffset * w + _zoom * width, _yOffset * h + _zoom * height, 0);
			glTexCoord3f(0, 1, 1);  glVertex3i(_xOffset * w, _yOffset * h + _zoom * height, 0);
		}
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	QString textContent("Change the text from the settings");

	textContent = _settings->textContent();
	int y = 200;

	if(_settings->displayStaticText()) {
		PhGraphicSolidRect rect1(50, 10, 500, 100);
		rect1.setColor(Qt::yellow);
		rect1.draw();
		PhGraphicText text1(&_font1, textContent);

		text1.setRect(50, 10, 500, 100);
		text1.setColor(Qt::green);
		text1.draw();
	}

	if(_settings->displayMovingText()) {
		PhGraphicText text2(&_font2, textContent);
		y += 200;

		_x += 4;
		if(_x > width - 500)
			_x = 0;
		text2.setRect(_x, y, 500, 100);
		text2.setColor(Qt::green);
		text2.draw();
	}

	if(_settings->displayRect()) {
		PhGraphicSolidRect rect3(0, 300, 400, 300);
		rect3.setColor(Qt::blue);
		rect3.draw();

		_yuvRect.draw();
		_rgbRect.draw();
	}


	if(_settings->displayLoops()) {
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
	}

	//_yuvRect.draw();

	if(_settings->displayDisc()) {
		PhGraphicDisc disc(300, 300, 100);
		disc.setColor(Qt::blue);
		disc.draw();
	}

	if(_settings->displayArrows()) {
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
	}
}

void GraphicTestWindow::on_actionInfos_triggered(bool checked)
{
	_settings->setDisplayInfo(checked);
}

void GraphicTestWindow::on_actionImage_triggered(bool checked)
{
	_settings->setDisplayImage(checked);
}

void GraphicTestWindow::on_actionLoops_triggered(bool checked)
{
	_settings->setDisplayLoops(checked);
}

void GraphicTestWindow::on_actionRectangles_triggered(bool checked)
{
	_settings->setDisplayRects(checked);
}

void GraphicTestWindow::on_actionArrows_triggered(bool checked)
{
	_settings->setDisplayArrows(checked);
}

void GraphicTestWindow::on_actionDisc_triggered(bool checked)
{
	_settings->setDisplayDisc(checked);
}

void GraphicTestWindow::on_actionCharacters_triggered(bool checked)
{
	_settings->setDisplayCharacters(checked);
}

void GraphicTestWindow::on_actionStatic_text_triggered(bool checked)
{
	_settings->setDisplayStaticText(checked);
}

void GraphicTestWindow::on_actionMoving_text_triggered(bool checked)
{
	_settings->setDisplayMovingText(checked);
}

void GraphicTestWindow::on_actionZoom_in_triggered()
{
	_zoom++;
}

void GraphicTestWindow::on_actionZoom_out_triggered()
{
	if(_zoom > 1)
		_zoom--;
}

void GraphicTestWindow::on_actionMove_up_triggered()
{
	_yOffset--;
}

void GraphicTestWindow::on_actionMove_right_triggered()
{
	_xOffset++;
}

void GraphicTestWindow::on_actionMove_down_triggered()
{
	_yOffset++;
}

void GraphicTestWindow::on_actionMove_left_triggered()
{
	_xOffset--;
}

void GraphicTestWindow::on_actionChange_text_triggered()
{
	ChangeTextDialog dlg(_settings->textContent(), this);
	QString oldTextContent = _settings->textContent();
	connect(&dlg, &ChangeTextDialog::textChanged, _settings, &GraphicTestSettings::setTextContent);
	if(dlg.exec() != QDialog::Accepted)
		_settings->setTextContent(oldTextContent);
}

void GraphicTestWindow::onFontSelected(const QFont &font)
{
	PHDEBUG << font.family() << font.weight();
	_font1.setFamily(font.family());
	_font1.setWeight(font.weight());
	_settings->setFont1Family(font.family());
	_settings->setTextWeight(font.weight());
}
