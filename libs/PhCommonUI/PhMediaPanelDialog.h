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

	bool isMousePressed();

private:
	Ui::PhMediaPanelDialog *ui;

	// For moving with mouse
	int _iXdeffarance;
	int _iYdeffarance;
	bool _mousePressed;

	void mousePressEvent( QMouseEvent * event);
	void mouseReleaseEvent( QMouseEvent * event );
	void mouseMoveEvent( QMouseEvent * event );
};

#endif // PHMEDIAPANELDIALOG_H
