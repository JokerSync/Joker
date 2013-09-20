#include "TimecodeEdit.h"

TimecodeEdit::TimecodeEdit(QWidget *parent) :
    QLineEdit(parent),
    _frame(0),
    _tcType(PhTimeCodeType25)
{

    connect(this, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));

}

TimecodeEdit::~TimecodeEdit()
{

}

void TimecodeEdit::setTCType(PhTimeCodeType tcType)
{
    _tcType = tcType;
}


void TimecodeEdit::onTextChanged(QString text)
{
    _frame = PhTimeCode::frameFromString(text, _tcType);
    //PhTimeCode::ComputeHhMmSsFf(hhmmssff, _frame, _tcType);

	emit frameChanged(_frame, _tcType);
}

void TimecodeEdit::isTimeCode(QString text)
{

}
