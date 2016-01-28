/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

#include "PhSync/PhTime.h"

namespace Ui {
class AboutDialog;
}
/**
 * @brief This dialog display legal and version information about Joker.
 */
class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief The AboutDialog constructor
	 * @param parent The parent object
	 */
	explicit AboutDialog(QWidget *parent = 0);
	~AboutDialog();
	void setTimePlayed(PhTime timePlayed);

private slots:
	void on_pushButton_clicked();

private:
	Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
