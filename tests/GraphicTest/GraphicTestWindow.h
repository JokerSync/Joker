#ifndef MAINWINDOW_H
#define MAINWINDOW_W

#include <QMainWindow>
#include "GraphicTestView.h"

namespace Ui {
class GraphicTestWindow;
}

class GraphicTestWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit GraphicTestWindow(QWidget *parent = 0);
	~GraphicTestWindow();

private slots:
	void on_actionChange_font_triggered();

private:
	Ui::GraphicTestWindow *ui;
};

#endif // MAINWINDOW_H
