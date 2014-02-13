/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}
/*!
 * \brief The AboutDialog class
 * This little class allow Joker to show informations about himself
 */
class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AboutDialog(QWidget *parent = 0);
	~AboutDialog();

private slots:
	void on_pushButton_clicked();

private:
	Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
