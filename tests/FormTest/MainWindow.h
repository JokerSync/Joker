#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QSettings>
#include <QGraphicsScene>

#include "AboutMenu.h"

namespace Ui {
class MainWindow;
}

class GoTo;
class AboutMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

	void generateRGB();

	void generateYUV();

	bool openFile(QString fileName);

protected:
	void paintEvent(QPaintEvent *event);
private slots:
    void on_actionAbout_triggered();

    void on_actionDocumentation_triggered();

	void on_actionOpen_triggered();

	void on_actionGenerate_YUV_pattern_triggered();

	void on_actionGenerate_RGB_pattern_triggered();

private:
    Ui::MainWindow *ui;
    AboutMenu _about;
	QSettings _settings;
	QImage *_image;
	unsigned char * _rgb;
};

#endif // MAINWINDOW_H
