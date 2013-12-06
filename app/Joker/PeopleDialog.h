#ifndef PEOPLEDIALOG_H
#define PEOPLEDIALOG_H

#include <QDialog>
#include <PhStrip/PhStripDoc.h>

namespace Ui {
class PeopleDialog;
}

class PeopleDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PeopleDialog(QWidget *parent, PhStripDoc* doc, QList<PhPeople*>* peopleList);

	~PeopleDialog();

private slots:
	void on_peopleList_itemSelectionChanged();

	void on_buttonBox_rejected();

	void on_selectAllButton_clicked();

private:
	Ui::PeopleDialog *ui;

	PhStripDoc* _doc;
	QList<PhPeople*> *_peopleList;
	QList<PhPeople*> _oldPeopleList;
};

#endif // PEOPLEDIALOG_H
