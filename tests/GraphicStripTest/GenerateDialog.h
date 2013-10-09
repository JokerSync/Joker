/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef GENERATEDIALOG_H
#define GENERATEDIALOG_H

#include <QDialog>

#include "PhStrip/PhStripDoc.h"

namespace Ui {
class GenerateDialog;
}

class GenerateDialog : public QDialog
{
	Q_OBJECT

public:
	explicit GenerateDialog(PhStripDoc * doc, QWidget *parent = 0);
	~GenerateDialog();

private slots:
	void onTextChanged();
	void onAccept();

private:
	Ui::GenerateDialog *ui;
	PhStripDoc * doc;
};

#endif // GENERATEDIALOG_H
