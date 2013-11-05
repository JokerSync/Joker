#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>
#include "PhStrip/PhStripDoc.h"
#include "PhVideo/PhVideoEngine.h"

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
	void setVideoEngine(PhVideoEngine *videoEngine);

protected:
	void showEvent(QShowEvent *);

private:
	Ui::PropertyDialog *ui;
	PhStripDoc *_doc;
	PhVideoEngine *_videoEngine;
};

#endif // PROPERTYDIALOG_H
