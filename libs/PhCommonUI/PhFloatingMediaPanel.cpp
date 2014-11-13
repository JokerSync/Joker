/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhFloatingMediaPanel.h"

PhFloatingMediaPanel::PhFloatingMediaPanel(QWidget *parent) :
	PhMediaPanel(parent),
	_mousePressed(false),
	_mousePressedLocation(-1, -1)
{
	// assign flags
	setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
}

void PhFloatingMediaPanel::mousePressEvent( QMouseEvent *event)
{
	_mousePressed = true;
	_mousePressedLocation = event->pos();
}

void PhFloatingMediaPanel::mouseReleaseEvent( QMouseEvent * )
{
	_mousePressed = false;
}

void PhFloatingMediaPanel::mouseMoveEvent( QMouseEvent *event)
{
	if(_mousePressed)
		this->move(event->globalPos() - _mousePressedLocation);
}

bool PhFloatingMediaPanel::isMousePressed()
{
	return _mousePressed;
}
