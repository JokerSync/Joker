#include "TimecodeEdit.h"

TimecodeEdit::TimecodeEdit(QWidget *parent) :
    QLineEdit(parent),
    _frame(0),
    _tcType(PhTimeCodeType25)
{

    connect(this, SIGNAL(editingFinished()), this, SLOT(isTimeCode()));

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
    _frame = PhTimeCode::frameFromString(this->text(), _tcType);
    //PhTimeCode::ComputeHhMmSsFf(hhmmssff, _frame, _tcType);

    emit frameChanged(_frame);
}
