/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhFontDialog.h"
#include "ui_PhFontDialog.h"
#include <QDir>
#include "PhTools/PhDebug.h"

PhFontDialog::PhFontDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhFontDialog)
{
	ui->setupUi(this);
	ui->plainTextEdit->setPlainText("Type any text here.");
	ui->label->setText("Preview");

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
		ui->listWidgetFont->addItem(fontName.split(".").at(0));
}

QString PhFontDialog::getFontSelected()
{
	if(ui->listWidgetFont->selectedItems().empty())
		return "";
	return ui->listWidgetFont->selectedItems().at(0)->text();
}

PhFontDialog::~PhFontDialog()
{
	delete ui;
}


void PhFontDialog::on_listWidgetFont_itemClicked(QListWidgetItem *item)
{
	ui->plainTextEdit->setFont(QFont::QFont(item->text()));
	ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText());

}

void PhFontDialog::on_listWidgetFont_currentItemChanged(QListWidgetItem *current, QListWidgetItem)
{
	ui->plainTextEdit->setFont(QFont::QFont(current->text()));
	ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText());
}

void PhFontDialog::on_buttonBox_accepted()
{
	this->close();
}
