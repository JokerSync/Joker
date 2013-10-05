#include "AboutMenu.h"
#include "ui_AboutMenu.h"

AboutMenu::AboutMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutMenu)
{
    ui->setupUi(this);
}

AboutMenu::~AboutMenu()
{
    delete ui;
}
