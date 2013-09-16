#include "TimeCodeInput.h"
#include "ui_TimeCodeInput.h"

TimeCodeInput::TimeCodeInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeCodeInput)
{
    ui->setupUi(this);
    _frame = 0;
    _tcType = PhTimeCodeType25;

    connect(ui->input, SIGNAL(textEdited(QString)), this, SLOT(isTimeCode(QString)));

}

TimeCodeInput::~TimeCodeInput()
{
    delete ui;
}

void TimeCodeInput::setTCType(PhTimeCodeType tcType)
{
    _tcType = tcType;
}

void TimeCodeInput::isTimeCode(const QString &text)
{
    _frame = PhTimeCode::frameFromString(text, _tcType);



    emit frameChanged(_frame);
}
