#include "TimeCodeInput.h"
#include "ui_TimeCodeInput.h"

TimeCodeInput::TimeCodeInput(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeCodeInput),
    _frame(0),
    _medialength(7500),
    _tcType(PhTimeCodeType25)
{
    ui->setupUi(this);

    connect(ui->input, SIGNAL(editingFinished()), this, SLOT(isTimeCode()));

}

TimeCodeInput::~TimeCodeInput()
{
    delete ui;
}

void TimeCodeInput::setTCType(PhTimeCodeType tcType)
{
    _tcType = tcType;
}

void TimeCodeInput::setMediaLength(PhFrame length)
{
    _medialength = length;
}

void TimeCodeInput::isTimeCode()
{
    bool test = true;
    unsigned int hhmmssff[4];
    QString text = ui->input->text();
    PhFrame frame = PhTimeCode::frameFromString(text, _tcType);
    PhTimeCode::ComputeHhMmSsFf(hhmmssff, frame, _tcType);

    qDebug()<<"frame:"<<frame<<" "<<hhmmssff[0]<<hhmmssff[1]<<hhmmssff[2]<<hhmmssff[3];

    if(frame > _medialength)
        test = false;

    if(hhmmssff[1] > 60)
        test = false;

    if(hhmmssff[2] > 60)
        test = false;

    if(hhmmssff[3] > (PhTimeCode::getFps(_tcType)-1))
        test = false;

    if(test == true)
        _frame = frame;

    else
        _frame = 0;

    emit frameChanged(_frame);
}
