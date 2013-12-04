#include "PeopleDialog.h"
#include "ui_PeopleDialog.h"
#include "PhTools/PhDebug.h"

PeopleDialog::PeopleDialog(QWidget *parent, PhStripDoc* doc) :
	QDialog(parent),
	ui(new Ui::PeopleDialog),
	_doc(doc)
{
	ui->setupUi(this);

	foreach (PhPeople* people, _doc->getPeoples().values()) {
		PHDEBUG << people->getName();
		ui->peopleList->addItem(people->getName());
	}
}

PhPeople *PeopleDialog::selectedPeople()
{
	QList<QListWidgetItem*> list = ui->peopleList->selectedItems();
	if(list.count() > 0)
		return _doc->getPeopleByName(list.first()->text());
	return NULL;
}

PeopleDialog::~PeopleDialog()
{
	delete ui;
}
