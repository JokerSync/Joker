/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QMessageBox>

#include "PeopleDialog.h"
#include "ui_PeopleDialog.h"
#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhColorPickerDialog.h"

PeopleDialog::PeopleDialog(QWidget *parent, PhStripDoc* doc, JokerSettings *settings) :
	QDialog(parent),
	ui(new Ui::PeopleDialog),
	_doc(doc),
	_settings(settings)
{
	ui->setupUi(this);

	QStringList peopleList = _settings->selectedPeopleNameList();
	foreach(QString name, peopleList) {
		_oldPeopleNameList.append(name);
	}

	foreach(PhPeople* people, _doc->peoples()) {
		if(people) {
			QString name = people->name();
			ui->peopleList->addItem(name);

			if(peopleList.contains(name))
				ui->peopleList->item(ui->peopleList->count() - 1)->setSelected(true);
		}
	}
	if(ui->peopleList->count() == 0) {
		ui->peopleList->addItem(tr("The list is empty..."));
		ui->peopleList->setDisabled(true);
	}

#warning /// @todo Check if fixed
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

	this->setGeometry(100, 500, width(), height());
}

PeopleDialog::~PeopleDialog()
{
	delete ui;
}

void PeopleDialog::on_peopleList_itemSelectionChanged()
{
	QStringList peopleNameList;

	foreach(QListWidgetItem* item, ui->peopleList->selectedItems()) {
		peopleNameList.append(item->text());
	}

	if(peopleNameList.count() < _doc->peoples().count())
		_settings->setSelectedPeopleNameList(peopleNameList);
}

void PeopleDialog::on_buttonBox_rejected()
{
	QStringList peopleNameList;
	foreach(QString name, _oldPeopleNameList)
	peopleNameList.append(name);

	_settings->setSelectedPeopleNameList(peopleNameList);
}

void PeopleDialog::on_selectAllButton_clicked()
{
	ui->peopleList->selectAll();
}

void PeopleDialog::on_deselectAllButton_clicked()
{
	ui->peopleList->clearSelection();
}

void PeopleDialog::on_changeCharButton_clicked()
{
	if(ui->peopleList->selectedItems().count() != 1) {
		QMessageBox msgBox;
		msgBox.setText(tr("You have to select one character."));
		msgBox.exec();
		return;
	}
	PhColorPickerDialog * dlg = new PhColorPickerDialog(_doc, ui->peopleList->selectedItems().first()->text());
	dlg->exec();

}
