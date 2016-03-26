#ifndef CHANGETEXTDIALOG_H
#define CHANGETEXTDIALOG_H

#include <QDialog>

namespace Ui {
class ChangeTextDialog;
}

class ChangeTextDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ChangeTextDialog(QString text, QWidget *parent = 0);
	~ChangeTextDialog();

signals:
	void textChanged(QString text);

private slots:
	void on_lineEdit_textChanged(const QString &text);

private:
	Ui::ChangeTextDialog *ui;
};

#endif // CHANGETEXTDIALOG_H
