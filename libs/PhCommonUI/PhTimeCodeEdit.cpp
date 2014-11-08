/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QRegExpValidator>
#include <QKeyEvent>
#include <QApplication>

#include "PhTimeCodeEdit.h"

PhTimeCodeEdit::PhTimeCodeEdit(QWidget *parent) :
	QLineEdit(parent),
	_tcType(PhTimeCodeType25),
	_oldTimeCode("00:00:00:00"),
	_selectedIndex(-1),
	_mousePressed(false),
	_mousePressedLocation(0, 0)
{
	_addedNumbers.clear();
	setText("00:00:00:00");
	connect(this, &PhTimeCodeEdit::textChanged, this, &PhTimeCodeEdit::onTextChanged);
	this->installEventFilter(this);
	//Only accept numbers and ":" it avoid the use of special
	//chars like ` or ^
	QRegExp rx("([0-9]|:){11}");
	setValidator(new QRegExpValidator(rx, this));
}

void PhTimeCodeEdit::setFrame(PhFrame frame, PhTimeCodeType tcType)
{
	setTime(frame * PhTimeCode::timePerFrame(tcType), tcType);
}

void PhTimeCodeEdit::setTime(PhTime time, PhTimeCodeType tcType)
{
	_tcType = tcType;
	this->setText(PhTimeCode::stringFromTime(time, tcType));
	if(_oldTimeCode.length() == 0)
		_oldTimeCode = this->text();
}

bool PhTimeCodeEdit::isTimeCode()
{
	PhFrame frame = PhTimeCode::frameFromString(this->text(), _tcType);
	QString text = PhTimeCode::stringFromFrame(frame, _tcType);

	if(text == this->text())
		return true;
	else
		return false;
}

PhFrame PhTimeCodeEdit::frame()
{
	return PhTimeCode::frameFromString(this->text(), _tcType);
}

PhTime PhTimeCodeEdit::time()
{
	return PhTimeCode::timeFromString(this->text(), _tcType);
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
	switch (event->type()) {
	case QEvent::KeyPress:
		{
			QKeyEvent *keyEvent = (QKeyEvent*)event;
			switch (keyEvent->key()) {
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
				_addedNumbers.push(keyEvent->key());
				compute(true);
				return true;
			case Qt::Key_Backspace:
				if(_addedNumbers.length()) {
					_addedNumbers.pop();
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
	case QEvent::MouseButtonPress:
		QApplication::setOverrideCursor(Qt::SizeVerCursor);
		_mousePressed = true;
		_mousePressedLocation = static_cast<QMouseEvent *>(event)->pos();
		_selectedIndex = (cursorPositionAt(_mousePressedLocation) / 3) * 3;

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
				PhFrame fps = PhTimeCode::getFps(_tcType);

				PhFrame offset = 0;
				switch(_selectedIndex) {
				case 0:
					offset = fps * 60 * 60;
					break;
				case 3:
					offset = fps * 60;
					break;
				case 6:
					offset = fps;
					break;
				case 9:
					offset = 1;
					break;
				}
				if(_mousePressedLocation.y() > y)
					currentFrame += offset;
				else
					currentFrame -= offset;

				_mousePressedLocation.setY(y);
				this->setText(PhTimeCode::stringFromFrame(currentFrame, _tcType));

				if(text().contains("-"))
					setSelection(_selectedIndex + 1, 2);
				else
					setSelection(_selectedIndex, 2);

				return true;
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
		currentText = _oldTimeCode;

	// Remove temporaly the ":"
	currentText.remove(":");

	int start = 0;
	if(_addedNumbers.size() > 8)
		start = _addedNumbers.size() - 8;
	for(int i = start; i < _addedNumbers.size(); i++)
		currentText.replace(i + 8 - _addedNumbers.size(), 1, _addedNumbers.at(i));

	// Re add the ":"
	currentText.insert(2, ":"); //xx:xxxxxx
	currentText.insert(5, ":"); //xx:xx:xxxx
	currentText.insert(8, ":"); //xx:xx:xx:xx

	this->setText(currentText);

	int textLength = this->text().length();
	int selectionLength = this->_addedNumbers.count();
	if(selectionLength > 6)
		selectionLength += 3;
	else if(selectionLength > 4)
		selectionLength += 2;
	else if(selectionLength > 2)
		selectionLength += 1;
	setSelection(textLength - selectionLength, textLength);

	onTextChanged(this->text());
}
