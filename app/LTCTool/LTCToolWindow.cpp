#include "LTCToolWindow.h"
#include "ui_LTCToolWindow.h"

LTCToolWindow::LTCToolWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::LTCToolWindow)
{
	ui->setupUi(this);
}

LTCToolWindow::~LTCToolWindow()
{
	delete ui;
}
