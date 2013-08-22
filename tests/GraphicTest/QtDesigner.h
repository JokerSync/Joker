#ifndef QTDESIGNER_H
#define QTDESIGNER_H

#include <QMainWindow>
#include "GraphicTestView.h"

namespace Ui {
class QtDesigner;
}

class QtDesigner : public QMainWindow
{
	Q_OBJECT

public:
	Ui::QtDesigner * getUi(){return ui;};

	
public:
	explicit QtDesigner(QWidget *parent = 0);
	~QtDesigner();

private:
	Ui::QtDesigner *ui;

};

#endif // QTDESIGNER_H
