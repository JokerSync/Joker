#ifndef PHMEDIAPANELDIALOG_H
#define PHMEDIAPANELDIALOG_H

#include <QDialog>

#include "PhTools/PhClock.h"

namespace Ui {
class PhMediaPanelDialog;
}

class PhMediaPanelDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PhMediaPanelDialog(QWidget *parent = 0);
	~PhMediaPanelDialog();

	void setClock(PhClock *clock);
	void setFirstFrame(PhFrame frame);
	void setMediaLength(PhFrame length);
    void setSliderEnable(bool isEnabled);

private:
	Ui::PhMediaPanelDialog *ui;
};

#endif // PHMEDIAPANELDIALOG_H
