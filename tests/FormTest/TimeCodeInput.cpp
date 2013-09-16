#include "TimeCodeInput.h"
#include "ui_TimeCodeInput.h"

TimeCodeInput::TimeCodeInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeCodeInput)
{
    ui->setupUi(this);
}

TimeCodeInput::~TimeCodeInput()
{
    delete ui;
}
