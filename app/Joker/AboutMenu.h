/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef ABOUTMENU_H
#define ABOUTMENU_H

#include <QDialog>

namespace Ui {
class AboutMenu;
}

class AboutMenu : public QDialog
{
	Q_OBJECT

public:
	explicit AboutMenu(QWidget *parent = 0);
	~AboutMenu();

private slots:
	void on_pushButton_clicked();

private:
	Ui::AboutMenu *ui;
};

#endif // ABOUTMENU_H
