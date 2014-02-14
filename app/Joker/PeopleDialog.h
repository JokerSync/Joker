/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PEOPLEDIALOG_H
#define PEOPLEDIALOG_H

#include <QDialog>
#include <PhStrip/PhStripDoc.h>

namespace Ui {
class PeopleDialog;
}
/*!
 * \brief PhPeople selection dialog.
 *
 * This modal dialog allow the final user to select or deselect any PhPeople coming from the PhStripDoc.
 * Selected people will be displayed on the left. Other people will be grayed out on the strip.
 * The next timecode in the upper right part of the application will displayed the next selected people time code.
 */
class PeopleDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
	 * \brief The PeopleDialog constructor
	 * \param parent The parent object
	 * \param doc The current PhStripDoc
	 * \param peopleList The current selected people list
	 */
	explicit PeopleDialog(QWidget *parent, PhStripDoc* doc, QList<PhPeople*>* peopleList);

	~PeopleDialog();

private slots:
	void on_peopleList_itemSelectionChanged();

	void on_buttonBox_rejected();

	void on_selectAllButton_clicked();

	void on_deselectAllButton_clicked();

private:
	Ui::PeopleDialog *ui;

	PhStripDoc* _doc;
	QList<PhPeople*> *_peopleList;
	QList<PhPeople*> _oldPeopleList;
};

#endif // PEOPLEDIALOG_H
