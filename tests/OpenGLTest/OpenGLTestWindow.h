#ifndef OPENGLTESTWINDOW_H
#define OPENGLTESTWINDOW_H

#include <QGLWidget>

class OpenGLTestWindow : public QGLWidget
{
	Q_OBJECT

public:
	explicit OpenGLTestWindow(QWidget *parent = 0);

	void initializeGL();
	void paintGL();

	void loadFile(const char * fileName);
};

#endif // OPENGLTESTWINDOW_H
