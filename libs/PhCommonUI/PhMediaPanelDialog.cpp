#include "PhMediaPanelDialog.h"
#include "ui_PhMediaPanelDialog.h"

PhMediaPanelDialog::PhMediaPanelDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhMediaPanelDialog),_iXdeffarace(-1), _iYdeffarance(-1)
{
	ui->setupUi(this);
	this->setFixedSize(ui->mediaPanel->size());
	setParent(0);
	Qt::WindowFlags flags = Qt::Widget;
	flags |= Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint;
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

void PhMediaPanelDialog::mousePressEvent ( QMouseEvent * event)
{
	_mousePressed = true;
	QPoint qpMousePressedPoint = QCursor::pos();
	QPoint qpApploc = this->pos();
	_iXdeffarace = qpMousePressedPoint.x() - qpApploc.x();
	_iYdeffarance = qpMousePressedPoint.y() - qpApploc.y();
}

//************************************************** ******
void PhMediaPanelDialog::mouseReleaseEvent ( QMouseEvent * event )
{
	_mousePressed = false;
}

//************************************************** ******
void PhMediaPanelDialog::mouseMoveEvent ( QMouseEvent * event )
{
	if(_mousePressed)
	{
		QPoint qpAppNewLoc( (QCursor::pos().x() - _iXdeffarace) , (QCursor::pos().y() - _iYdeffarance) );
		this->setProperty("pos", qpAppNewLoc);
	}
}

bool PhMediaPanelDialog::isMousePressed()
{
	return _mousePressed;
}
