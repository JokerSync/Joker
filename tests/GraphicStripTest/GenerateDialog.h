/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef GENERATEDIALOG_H
#define GENERATEDIALOG_H

#include <QDialog>

#include "PhStrip/PhStripDoc.h"
#include "PhCommonUI/PhTimeCodeEdit.h"

#include "GraphicStripTestSettings.h"

namespace Ui {
class GenerateDialog;
}

class GenerateDialog : public QDialog
{
	Q_OBJECT

public:
	explicit GenerateDialog(GraphicStripTestSettings *settings, PhStripDoc * _doc, QWidget *parent = 0);
	~GenerateDialog();
	PhTime getTC();
	bool getCheckBoxState();

private slots:
	void onTextChanged();
	void onAccept();

private:
	Ui::GenerateDialog *ui;
	PhStripDoc * _doc;
	GraphicStripTestSettings * _settings;
};

#endif // GENERATEDIALOG_H
