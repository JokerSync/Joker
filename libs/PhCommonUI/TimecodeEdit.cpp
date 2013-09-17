#include "TimecodeEdit.h"

TimecodeEdit::TimecodeEdit(QLineEdit *parent) :
    QLineEdit(parent),
    _frame(0),
    _tcType(PhTimeCodeType25)
{

    //connect(ui->input, SIGNAL(editingFinished()), this, SLOT(isTimeCode()));

}

TimecodeEdit::~TimecodeEdit()
{

}

void TimecodeEdit::setTCType(PhTimeCodeType tcType)
{
    _tcType = tcType;
}


void TimecodeEdit::isTimeCode()
{
    bool test = true;
    unsigned int hhmmssff[4];
    QString text = NULL;
    PhFrame frame = PhTimeCode::frameFromString(text, _tcType);
    PhTimeCode::ComputeHhMmSsFf(hhmmssff, frame, _tcType);

    qDebug()<<"frame:"<<frame<<" "<<hhmmssff[0]<<hhmmssff[1]<<hhmmssff[2]<<hhmmssff[3];

    emit frameChanged(_frame);
}
