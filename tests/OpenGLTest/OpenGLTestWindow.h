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

	void resizeGL(int width, int height);

private:
	QTimer _refreshTimer;
	QTime _timer;
	unsigned int _texture1, _texture2;
};

#endif // OPENGLTESTWINDOW_H
