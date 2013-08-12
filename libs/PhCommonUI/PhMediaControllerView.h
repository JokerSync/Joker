#ifndef PHMEDIACONTROLLERVIEW_H
#define PHMEDIACONTROLLERVIEW_H

#include <QMainWindow>

namespace Ui {
class PhMediaControllerView;
}

class PhMediaControllerView : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit PhMediaControllerView(QWidget *parent = 0);
	~PhMediaControllerView();
	
private:
	Ui::PhMediaControllerView *ui;
};

#endif // PHMEDIACONTROLLERVIEW_H
