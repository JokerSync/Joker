#include "Dialog.h"
#include "ui_Dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->lineEdit, SIGNAL(frameChanged(PhFrame)), this, SIGNAL(frameChanged(PhFrame)));
}

Dialog::~Dialog()
{
    delete ui;
}
