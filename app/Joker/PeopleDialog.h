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
	explicit PeopleDialog(QWidget *parent, PhStripDoc* doc);

	PhPeople* selectedPeople();

	~PeopleDialog();

private:
	Ui::PeopleDialog *ui;

	PhStripDoc* _doc;
};

#endif // PEOPLEDIALOG_H
