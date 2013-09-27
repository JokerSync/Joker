#ifndef STRIPPROPERTIESDIALOG_H
#define STRIPPROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
class StripPropertiesDialog;
}

class StripPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit StripPropertiesDialog(QString title, QString path, QString ips, QWidget *parent = 0);
	~StripPropertiesDialog();

private:
	Ui::StripPropertiesDialog *ui;
};

#endif // STRIPPROPERTIESDIALOG_H
