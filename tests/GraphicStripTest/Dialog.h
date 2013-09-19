#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLayout>

#ifndef DIALOG_H
#define DIALOG_H

class Dialog : public QDialog
{
public:
	Dialog(QWidget *parent=0);
	QPushButton * getGoToBtn(){return goToBtn;};
	QLineEdit * getTextLineEdit(){return textLineEdit;};

private:
	QPushButton *goToBtn;
	QLineEdit *textLineEdit;
};

#endif // DIALOG_H
