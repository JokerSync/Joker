/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PEOPLEDIALOG_H
#define PEOPLEDIALOG_H

#include <QDialog>

#include "PhStrip/PhStripDoc.h"

#include "JokerSettings.h"

namespace Ui {
class PeopleDialog;
}
/**
 * @brief PhPeople selection dialog.
 *
 * This modal dialog allow the final user to select or deselect any PhPeople coming from the PhStripDoc.
 * Selected people will be displayed on the left. Other people will be grayed out on the strip.
 * The next timecode in the upper right part of the application will displayed the next selected people time code.
 */
class PeopleDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief The PeopleDialog constructor
	 *
	 * By giving the PeopleDialog the application settings, the dialog
	 * is able to update in real time the PhGraphicStrip display.
	 *
	 * @param parent The parent object
	 * @param doc The current PhStripDoc
	 * @param settings The application settings
	 */
	explicit PeopleDialog(QWidget *parent, PhStripDoc* doc, JokerSettings *settings);

	~PeopleDialog();

private slots:
	void on_peopleList_itemSelectionChanged();

	void on_buttonBox_rejected();

	void on_deselectAllButton_clicked();

	void on_changeCharButton_clicked();

private:
	Ui::PeopleDialog *ui;
	PhStripDoc* _doc;
	JokerSettings *_settings;
	QStringList _oldPeopleNameList;
};

#endif // PEOPLEDIALOG_H
