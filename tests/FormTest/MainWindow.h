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

	bool openFile(QString fileName);

private slots:
    void on_actionAbout_triggered();

    void on_actionDocumentation_triggered();

	void on_actionOpen_triggered();

private:
    Ui::MainWindow *ui;
    AboutMenu _about;
	QSettings _settings;
	QGraphicsScene scene;
};

#endif // MAINWINDOW_H
