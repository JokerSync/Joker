#ifndef STRIPPROPERTIESDIALOG_H
#define STRIPPROPERTIESDIALOG_H

#include <QDialog>
#include "PhStrip/PhStripDoc.h"

namespace Ui {
class StripPropertiesDialog;
}

class StripPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit StripPropertiesDialog(PhStripDoc * doc, QWidget *parent = 0);
	~StripPropertiesDialog();

private:
	Ui::StripPropertiesDialog *ui;
};

#endif // STRIPPROPERTIESDIALOG_H
