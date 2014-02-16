#include "PhTimecodeEdit.h"

#include <QRegExpValidator>

PhTimeCodeEdit::PhTimeCodeEdit(QWidget *parent) :
    QLineEdit(parent),
	_tcType(PhTimeCodeType25)
{
	connect(this, SIGNAL(textChanged(QString)), this, SLOT(onTextChanged(QString)));
	this->installEventFilter(this);
	//Only accept numbers and ":" it avoid the use of special
	//chars like ` or ^
	QRegExp rx("([0-9]|:){11}");
	setValidator(new QRegExpValidator(rx, this));
}

void PhTimeCodeEdit::setFrame(PhFrame frame, PhTimeCodeType tcType)
{
	_tcType = tcType;
	this->setText(PhTimeCode::stringFromFrame(frame, tcType));
	if(_oldFrame.length() == 0)
		_oldFrame = this->text();

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

bool PhTimeCodeEdit::eventFilter(QObject *, QEvent *event)
{
	if(event->type() == QEvent::KeyPress)
	{
		int keyPressed = static_cast<QKeyEvent *>(event)->key();
		switch (keyPressed) {
		case Qt::Key_0:
		case Qt::Key_1:
		case Qt::Key_2:
		case Qt::Key_3:
		case Qt::Key_4:
		case Qt::Key_5:
		case Qt::Key_6:
		case Qt::Key_7:
		case Qt::Key_8:
		case Qt::Key_9:
			if(_addedNumbers.length() < 8)
			{
				_addedNumbers.append(QString::number(keyPressed % 0x30));
				compute(true);
			}
			return true;
		case Qt::Key_Backspace:
			if(_addedNumbers.length())
			{
				_addedNumbers.remove(_addedNumbers.length() - 1, 1);
				compute(false);
			}
			return true;
		case Qt::Key_Escape:
		case Qt::Key_Enter:
		case Qt::Key_Return:
			return false;
		default:
			return true;
		}

	}
	return false;
}

void PhTimeCodeEdit::compute(bool add)
{
	QString currentText;
	if(add)
		 currentText = this->text();
	else
		currentText = _oldFrame;
	currentText.remove(":");

	int i = 0;
	while(i < _addedNumbers.length())
	{
		currentText.replace(currentText.length() - 1 - i, 1, _addedNumbers.at(_addedNumbers.length() - 1 - i));
		i++;
	}
	//Current text is xxxxxxxx
	currentText.insert(2, ":");
	//xx:xxxxxx
	currentText.insert(5, ":");
	//xx:xx:xxxx
	currentText.insert(8, ":");
	//xx:xx:xx:xx

	this->setText(currentText);
	onTextChanged(this->text());
}

