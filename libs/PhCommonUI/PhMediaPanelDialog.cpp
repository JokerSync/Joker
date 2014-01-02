#include "PhMediaPanelDialog.h"
#include "ui_PhMediaPanelDialog.h"

PhMediaPanelDialog::PhMediaPanelDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhMediaPanelDialog),_iXdeffarance(-1), _iYdeffarance(-1)
{
	ui->setupUi(this);
	// Set the size with the child's size
	setFixedSize(ui->mediaPanel->size());
	// Remove parent to make a top widget (flags compatibility)
	setParent(0);
	// assign flags
	Qt::WindowFlags flags = Qt::Widget | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint;
	setWindowFlags(flags);
}

PhMediaPanelDialog::~PhMediaPanelDialog()
{
	delete ui;
}

void PhMediaPanelDialog::setClock(PhClock *clock)
{
	ui->mediaPanel->setClock(clock);
}

void PhMediaPanelDialog::setFirstFrame(PhFrame frame)
{
	ui->mediaPanel->setFirstFrame(frame);
}

void PhMediaPanelDialog::setMediaLength(PhFrame length)
{
	ui->mediaPanel->setMediaLength(length);
}

void PhMediaPanelDialog::mousePressEvent ( QMouseEvent *)
{
	_mousePressed = true;
	QPoint qpMousePressedPoint = QCursor::pos();
	QPoint qpApploc = this->pos();
	_iXdeffarance = qpMousePressedPoint.x() - qpApploc.x();
	_iYdeffarance = qpMousePressedPoint.y() - qpApploc.y();
}

void PhMediaPanelDialog::mouseReleaseEvent ( QMouseEvent * )
{
	_mousePressed = false;
}

void PhMediaPanelDialog::mouseMoveEvent ( QMouseEvent * )
{
	if(_mousePressed)
	{
		QPoint qpAppNewLoc( (QCursor::pos().x() - _iXdeffarance) , (QCursor::pos().y() - _iYdeffarance) );
		setProperty("pos", qpAppNewLoc);
	}
}

bool PhMediaPanelDialog::isMousePressed()
{
	return _mousePressed;
}
