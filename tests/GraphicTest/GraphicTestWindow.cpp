#include <QFileDialog>
#include <QMessageBox>

#include "GraphicTestWindow.h"
#include "ui_GraphicTestWindow.h"

GraphicTestWindow::GraphicTestWindow(GraphicTestSettings *settings) :
	PhWindow(settings),
	ui(new Ui::GraphicTestWindow),
	_settings(settings)
{
	ui->setupUi(this);
	ui->graphicView->setTestSettings(_settings);
}

GraphicTestWindow::~GraphicTestWindow()
{
	delete ui;
}


void GraphicTestWindow::on_actionChange_font_triggered()
{
	QString fileName = QFileDialog::getOpenFileName();
	if(QFile(fileName).exists()) {
		if(!ui->graphicView->setFontFile(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}
