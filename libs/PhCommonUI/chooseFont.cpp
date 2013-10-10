/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "chooseFont.h"
#include "ui_chooseFont.h"
#include <QDir>
#include "PhTools/PhDebug.h"

chooseFont::chooseFont(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::chooseFont)
{
	ui->setupUi(this);


	QStringList fontList;
	QDir myDir("/Library/Fonts/");
	QDir myDir2("~/Library/Fonts/");
	QStringList filters;
	filters.append("*.ttf");
	myDir.setNameFilters(filters);
	fontList = myDir.entryList();
	fontList += myDir2.entryList();
	fontList.sort();
	fontList.removeDuplicates();
	foreach(QString font, fontList)
	{
		if(font.contains(" "))
			fontList.removeOne(font);
	}


	foreach(QString font, fontList)
		ui->listWidgetFont->addItem(font.split(".").at(0));
}

QString chooseFont::getFontSelected()
{
	return ui->listWidgetFont->selectedItems().at(0)->text();
}

chooseFont::~chooseFont()
{
	delete ui;
}


void chooseFont::on_listWidgetFont_itemClicked(QListWidgetItem *item)
{
	ui->textBrowser->setFont(QFont::QFont(item->text()));
	ui->textBrowser->setText(ui->textBrowser->toPlainText());

}

void chooseFont::on_listWidgetFont_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	ui->textBrowser->setFont(QFont::QFont(current->text()));
	ui->textBrowser->setText(ui->textBrowser->toPlainText());
}

void chooseFont::on_buttonBox_accepted()
{
	this->close();
}
