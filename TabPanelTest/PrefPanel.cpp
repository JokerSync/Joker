/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PrefPanel.h"
#include "ui_PrefPanel.h"

PrefPanel::PrefPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PrefPanel)
{
	ui->setupUi(this);
	this->setMaximumHeight(this->height());
	this->setMaximumWidth(this->width());
}

PrefPanel::~PrefPanel()
{
	delete ui;
}
