#ifndef PHTIMECODEDIALOG_H
#define PHTIMECODEDIALOG_H

#include <QDialog>

#include "PhCommonUI/TimecodeEdit.h"
#include "PhTools/PhTimeCode.h"

namespace Ui {
class PhTimeCodeDialog;
}

class PhTimeCodeDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PhTimeCodeDialog(QWidget *parent = 0);
	~PhTimeCodeDialog();

	void setFrame(QString frame);

signals:
	void frameChanged(PhFrame frame, PhTimeCodeType tcType);

private:
	Ui::PhTimeCodeDialog *ui;
};

#endif // PHTIMECODEDIALOG_H
