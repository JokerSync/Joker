#include "AboutMenu.h"
#include "ui_AboutMenu.h"

AboutMenu::AboutMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutMenu)
{
    ui->setupUi(this);
	QString version = APP_NAME;
	version += " v";
	version += APP_VERSION;
	ui->label->setText(version);
}

AboutMenu::~AboutMenu()
{
    delete ui;
}
