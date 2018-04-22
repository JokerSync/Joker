/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PeopleDialog.h"
#include "ui_PeopleDialog.h"
#include "PhTools/PhDebug.h"
#include "PhStrip/PhPeople.h"

#include "PeopleEditionDialog.h"

PeopleDialog::PeopleDialog(QWidget *parent, PhStripDoc* doc, JokerSettings *settings, QStringListModel *selectedPeopleModel) :
	QDialog(parent),
	ui(new Ui::PeopleDialog),
	_doc(doc),
	_settings(settings),
	_selectedPeopleModel(selectedPeopleModel)
{
	ui->setupUi(this);

	QStringList peopleList = _settings->selectedPeopleNameList();
	foreach(QString name, peopleList) {
		_oldPeopleNameList.append(name);
	}

	QListIterator<PhPeople*> i = _doc->peopleModel()->iterator();
	while (i.hasNext()) {
		PhPeople* people = i.next();
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

	// TODO Check if fixed
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));

	ui->changeCharButton->setEnabled(ui->peopleList->selectedItems().count() == 1);

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

	if(peopleNameList.count() < _doc->peopleModel()->rowCount()) {
		_settings->setSelectedPeopleNameList(peopleNameList);
		_selectedPeopleModel->setStringList(peopleNameList);
	}

	ui->changeCharButton->setEnabled(ui->peopleList->selectedItems().count() == 1);
}

void PeopleDialog::on_buttonBox_rejected()
{
	QStringList peopleNameList;
	foreach(QString name, _oldPeopleNameList)
		peopleNameList.append(name);

	_settings->setSelectedPeopleNameList(peopleNameList);
	_selectedPeopleModel->setStringList(peopleNameList);
}

void PeopleDialog::on_deselectAllButton_clicked()
{
	ui->peopleList->clearSelection();
}

void PeopleDialog::on_changeCharButton_clicked()
{
	PhPeople * people = _doc->peopleByName(ui->peopleList->selectedItems().first()->text());
	PeopleEditionDialog * dlg = new PeopleEditionDialog(_doc, people);
	dlg->exec();
}
