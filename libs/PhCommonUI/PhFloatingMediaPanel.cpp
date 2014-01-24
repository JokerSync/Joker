#include "PhFloatingMediaPanel.h"

PhFloatingMediaPanel::PhFloatingMediaPanel(QWidget *parent) :
	PhMediaPanel(parent),
	_iXdeffarance(-1), _iYdeffarance(-1)
{
	// Remove parent to make a top widget (flags compatibility)
	setParent(0);
	// assign flags
	Qt::WindowFlags flags = Qt::Widget | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint;
	setWindowFlags(flags);
}

void PhFloatingMediaPanel::mousePressEvent ( QMouseEvent *)
{
	_mousePressed = true;
	QPoint qpMousePressedPoint = QCursor::pos();
	QPoint qpApploc = this->pos();
	_iXdeffarance = qpMousePressedPoint.x() - qpApploc.x();
	_iYdeffarance = qpMousePressedPoint.y() - qpApploc.y();
}

void PhFloatingMediaPanel::mouseReleaseEvent ( QMouseEvent * )
{
	_mousePressed = false;
}

void PhFloatingMediaPanel::mouseMoveEvent ( QMouseEvent * )
{
	if(_mousePressed)
	{
		QPoint qpAppNewLoc( (QCursor::pos().x() - _iXdeffarance) , (QCursor::pos().y() - _iYdeffarance) );
		setProperty("pos", qpAppNewLoc);
	}
}

bool PhFloatingMediaPanel::isMousePressed()
{
	return _mousePressed;
}
