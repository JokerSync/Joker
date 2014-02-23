#include <QFileDialog>
#include <QMessageBox>

#include "GraphicTestWindow.h"
#include "ui_GraphicTestWindow.h"

GraphicTestWindow::GraphicTestWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::GraphicTestWindow)
{
	ui->setupUi(this);
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
