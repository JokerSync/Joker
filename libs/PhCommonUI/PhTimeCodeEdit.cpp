/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTimecodeEdit.h"

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
	_mousePressed = false;
	_selectedIndex = 0;
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
	if(isTimeCode()) {
		PhFrame frame = PhTimeCode::frameFromString(text, _tcType);
		emit frameChanged(frame, _tcType);
	}
}

bool PhTimeCodeEdit::eventFilter(QObject *, QEvent *event)
{
	int keyPressed;
	switch (event->type()) {
	case QEvent::KeyPress:
		keyPressed = static_cast<QKeyEvent *>(event)->key();
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
			if(_addedNumbers.length() < 8) {
				_addedNumbers.append(QString::number(keyPressed % 0x30));
				compute(true);
			}
			return true;
		case Qt::Key_Backspace:
			if(_addedNumbers.length()) {
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
	case QEvent::MouseButtonPress:
		QApplication::setOverrideCursor(Qt::SizeVerCursor);
		_mousePressed = true;
		_mousePressedLocation = static_cast<QMouseEvent *>(event)->pos();
#warning /// @todo make it font size independant
		if(_mousePressedLocation.x() > 110 and _mousePressedLocation.x() < 145) {
			_selectedIndex = 0;
		}
		else if(_mousePressedLocation.x() > 145 and _mousePressedLocation.x() < 190) {
			_selectedIndex = 3;
		}
		else if(_mousePressedLocation.x() > 190 and _mousePressedLocation.x() < 230) {
			_selectedIndex = 6;
		}
		else if(_mousePressedLocation.x() > 230 and _mousePressedLocation.x() < 270) {
			_selectedIndex = 9;
		}
		return true;
	case QEvent::MouseButtonRelease:
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		_mousePressed = false;
		return true;
	case QEvent::MouseMove:
		{
			if(_mousePressed) {
				int y = static_cast<QMouseEvent *>(event)->pos().y();
				PhFrame currentFrame = PhTimeCode::frameFromString(this->text(), _tcType);

				if(_selectedIndex == 0) {
					if(_mousePressedLocation.y() > y)
						currentFrame += 25 * 60 * 60;
					else
						currentFrame -= 25 * 60 * 60;
				}
				else if(_selectedIndex == 3) {
					if(_mousePressedLocation.y() > y)
						currentFrame += 25 * 60;
					else
						currentFrame -= 25 * 60;
				}
				else if(_selectedIndex == 6) {
					if(_mousePressedLocation.y() > y)
						currentFrame += 25;
					else
						currentFrame -= 25;
				}
				else if(_selectedIndex == 9) {
					if(_mousePressedLocation.y() > y)
						currentFrame++;
					else
						currentFrame--;
				}

				_mousePressedLocation.setY(y);
				this->setText(PhTimeCode::stringFromFrame(currentFrame, _tcType));
			}
			return false;
		}

	default:
		return false;
	}
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
	while(i < _addedNumbers.length()) {
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

void PhTimeCodeEdit::paintEvent(QPaintEvent *e)
{
	if(_mousePressed) {
		if(text().contains("-"))
			setSelection(_selectedIndex + 1, 2);
		else
			setSelection(_selectedIndex, 2);
	}
	else
		deselect();
	QLineEdit::paintEvent(e);
}


