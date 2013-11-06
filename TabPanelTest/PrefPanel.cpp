/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include <QDir>
#include "PrefPanel.h"
#include "ui_PrefPanel.h"

PrefPanel::PrefPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PrefPanel)
{
	ui->setupUi(this);
	this->setMaximumHeight(this->height());
	this->setMaximumWidth(this->width());
	ui->tabWidget->setCurrentIndex(0);
	QStringList fontList;
	QDir systemFont("/Library/Fonts/");
	QDir userFont("~/Library/Fonts/");



	QStringList filters;
	filters.append("*.ttf");
	systemFont.setNameFilters(filters);
	fontList = systemFont.entryList();
	fontList += userFont.entryList();
	fontList.sort();
	fontList.removeDuplicates();
	foreach(QString fontFullName, fontList)
	{
		if(fontFullName.contains(" "))
			fontList.removeOne(fontFullName);
	}
	foreach(QString fontName, fontList)
		ui->listWidgetFont->addItem(fontName);
}

PrefPanel::~PrefPanel()
{
	delete ui;
}


void PrefPanel::on_buttonBox_accepted()
{
	close();
}

void PrefPanel::on_buttonBox_rejected()
{
	close();
}
