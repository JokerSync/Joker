#include "PhMediaPanelDialog.h"
#include "ui_PhMediaPanelDialog.h"

PhMediaPanelDialog::PhMediaPanelDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhMediaPanelDialog)
{
	ui->setupUi(this);
	this->setFixedSize(600, 95);
	Qt::WindowFlags flags = this->windowFlags();
	flags |= Qt::WindowStaysOnTopHint;
//	flags &= ~Qt::WindowFullscreenButtonHint;
//	flags &= ~Qt::WindowMinMaxButtonsHint;
	this->setWindowFlags(flags);
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
