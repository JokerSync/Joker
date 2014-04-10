#ifndef OPENGLTESTWINDOW_H
#define OPENGLTESTWINDOW_H

#include <QGLWidget>
#include <QTimer>
#include <QTime>

class OpenGLTestWindow : public QGLWidget
{
	Q_OBJECT

public:
	explicit OpenGLTestWindow(QWidget *parent = 0);

	void initializeGL();
	void paintGL();

private:
	QTimer _refreshTimer;
	QTime _timer;
};

#endif // OPENGLTESTWINDOW_H
