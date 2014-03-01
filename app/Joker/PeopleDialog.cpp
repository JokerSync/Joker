/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PeopleDialog.h"
#include "ui_PeopleDialog.h"
#include "PhTools/PhDebug.h"

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

	foreach(PhPeople* people, _doc->getPeoples().values()) {
		QString name = people->getName();
		ui->peopleList->addItem(name);

		if(peopleList.contains(name))
			ui->peopleList->item(ui->peopleList->count() - 1)->setSelected(true);
	}

#warning /// @todo Check if fixed
	ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Ok"));
	ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
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

	if(peopleNameList.count() < _doc->getPeoples().count())
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
