/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef CHOOSEFONT_H
#define CHOOSEFONT_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class PhFontDialog;
}

class PhFontDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PhFontDialog(QWidget *parent = 0);
	~PhFontDialog();
	QString getFontSelected();


private slots:

	void on_listWidgetFont_itemClicked(QListWidgetItem *item);

	void on_listWidgetFont_currentItemChanged(QListWidgetItem *current, QListWidgetItem);

	void on_buttonBox_accepted();

private:
	Ui::PhFontDialog *ui;
};

#endif // CHOOSEFONT_H
