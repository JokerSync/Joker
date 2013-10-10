/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef CHOOSEFONT_H
#define CHOOSEFONT_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class chooseFont;
}

class chooseFont : public QDialog
{
	Q_OBJECT

public:
	explicit chooseFont(QWidget *parent = 0);
	~chooseFont();
	QString getFontSelected();


private slots:

	void on_listWidgetFont_itemClicked(QListWidgetItem *item);

	void on_listWidgetFont_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

	void on_buttonBox_accepted();

private:
	Ui::chooseFont *ui;
};

#endif // CHOOSEFONT_H
