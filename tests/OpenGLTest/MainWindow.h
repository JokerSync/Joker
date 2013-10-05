#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGLWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QGLWidget
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);

	void initializeGL();
	void paintGL();

	void loadFile(const char * fileName);
};

#endif // MAINWINDOW_H
