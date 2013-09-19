#include "GoTo.h"
#include "ui_GoTo.h"

GoTo::GoTo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GoTo)
{
    ui->setupUi(this);
    connect(ui->_timecodeEdit, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)));
}

GoTo::~GoTo()
{
    delete ui;
}
