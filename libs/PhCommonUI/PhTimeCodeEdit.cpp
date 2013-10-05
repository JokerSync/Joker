#include "PhTimecodeEdit.h"

PhTimeCodeEdit::PhTimeCodeEdit(QWidget *parent) :
    QLineEdit(parent),
    _tcType(PhTimeCodeType25)
{

	connect(this, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));

}

void PhTimeCodeEdit::setFrame(PhFrame frame, PhTimeCodeType tcType)
{
	_tcType = tcType;
	this->setText(PhTimeCode::stringFromFrame(frame, tcType));
}

bool PhTimeCodeEdit::isTimeCode()
{
	PhFrame frame;
	QString text;
	frame = PhTimeCode::frameFromString(this->text(),_tcType);
	text = PhTimeCode::stringFromFrame(frame, _tcType);

	if(text == this->text())
		return true;
	else
		return false;
}

PhFrame PhTimeCodeEdit::frame()
{
	return PhTimeCode::frameFromString(this->text(), _tcType);
}

void PhTimeCodeEdit::onTextChanged(QString text)
{
	if(isTimeCode())
	{
		PhFrame frame = PhTimeCode::frameFromString(text, _tcType);
		emit frameChanged(frame, _tcType);
	}
}

