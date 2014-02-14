#include <QFileDialog>
#include <QMessageBox>

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_actionChange_font_triggered()
{
	QString fileName = QFileDialog::getOpenFileName();
	if(QFile(fileName).exists()) {
		if(!ui->graphicView->setFontFile(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}
