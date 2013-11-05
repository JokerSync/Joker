/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PREFPANEL_H
#define PREFPANEL_H

#include <QWidget>

namespace Ui {
class PrefPanel;
}

class PrefPanel : public QWidget
{
	Q_OBJECT

public:
	explicit PrefPanel(QWidget *parent = 0);
	~PrefPanel();

private slots:


	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

private:
	Ui::PrefPanel *ui;
};

#endif // PREFPANEL_H
