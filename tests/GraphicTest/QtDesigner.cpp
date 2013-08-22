#include "QtDesigner.h"
#include "ui_QtDesigner.h"


QtDesigner::QtDesigner(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::QtDesigner)
{
	ui->setupUi(this);
	ui->Play->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

QtDesigner::~QtDesigner()
{
	delete ui;
}

