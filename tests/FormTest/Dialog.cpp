#include "Dialog.h"
#include "ui_Dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->_timecodeEdit, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)));
}

Dialog::~Dialog()
{
    delete ui;
}
