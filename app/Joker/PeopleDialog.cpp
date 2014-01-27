#include "PeopleDialog.h"
#include "ui_PeopleDialog.h"
#include "PhTools/PhDebug.h"

PeopleDialog::PeopleDialog(QWidget *parent, PhStripDoc* doc, QList<PhPeople*> *peopleList) :
	QDialog(parent),
	ui(new Ui::PeopleDialog),
	_doc(doc),
	_peopleList(peopleList)
{
	ui->setupUi(this);

	foreach (PhPeople* people, *peopleList) {
		_oldPeopleList.append(people);
	}

	foreach (PhPeople* people, _doc->getPeoples().values()) {
		ui->peopleList->addItem(people->getName());

		if(_oldPeopleList.contains(people))
			ui->peopleList->item(ui->peopleList->count() - 1)->setSelected(true);
	}

	ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
}

PeopleDialog::~PeopleDialog()
{
	delete ui;
}

void PeopleDialog::on_peopleList_itemSelectionChanged()
{
    _peopleList->clear();
	foreach (QListWidgetItem* item, ui->peopleList->selectedItems()) {
		_peopleList->append(_doc->getPeopleByName(item->text()));
	}
	if(_peopleList->count() == _doc->getPeoples().count())
		_peopleList->clear();
}

void PeopleDialog::on_buttonBox_rejected()
{
    _peopleList->clear();
	foreach (PhPeople* people, _oldPeopleList) {
		_peopleList->append(people);
	}
}

void PeopleDialog::on_selectAllButton_clicked()
{
	ui->peopleList->selectAll();
}
