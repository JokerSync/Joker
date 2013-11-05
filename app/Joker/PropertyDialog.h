#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>
#include "PhStrip/PhStripDoc.h"

namespace Ui {
class PropertyDialog;
}

class PropertyDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PropertyDialog(QWidget *parent = 0);
	~PropertyDialog();

	void setDoc(PhStripDoc * doc);

protected:
	void showEvent(QShowEvent *);

private:
	Ui::PropertyDialog *ui;
	PhStripDoc *_doc;
};

#endif // PROPERTYDIALOG_H
