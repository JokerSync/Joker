#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>

#ifndef DIALOG_H
#define DIALOG_H

class TimeCodeDlg : public QDialog
{
public:
	TimeCodeDlg(QWidget *parent=0);
	QPushButton * getGoToBtn(){return goToBtn;};
	QString getTextLineEdit(){return textLineEdit->text();};

private:
	QPushButton *goToBtn;
	QLineEdit *textLineEdit;
};

#endif // DIALOG_H
