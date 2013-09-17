#include "Dialog.h"

#ifndef MAINDIALOG_H
#define MAINDIALOG_H

class MainDialog : public QDialog
{
	Q_OBJECT

public:
	MainDialog(QWidget *parent=0);

private slots:
	void showDialogModal();

private:
	Dialog *_dlg;
};
#endif // MAINDIALOG_H
