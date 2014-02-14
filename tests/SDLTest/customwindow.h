#include <QtWidgets>

#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H


class CustomWindow : public QWidget
{
	Q_OBJECT

public:
	CustomWindow(QWidget *parent = 0);
	~CustomWindow();

protected:
	void showEvent(QShowEvent *event);

private:
	void CenterOnScreen();
};

#endif

