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

bool TimecodeEdit::isTimeCode(QString timecode)
{
	PhFrame frame;
	QString text;
	frame = PhTimeCode::frameFromString(timecode,_tcType);
	text = PhTimeCode::stringFromFrame(frame, _tcType);

	qDebug() << text <<"/"<< timecode;

	if(text == timecode)
		return true;
	else
		return false;
}

void TimecodeEdit::onTextChanged(QString text)
{
	if(isTimeCode(text))
	{
		_frame = PhTimeCode::frameFromString(text, _tcType);
		emit frameChanged(_frame, _tcType);
	}
}

