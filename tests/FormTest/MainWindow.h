#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QSettings>

#include "PhTools/PhTimeCode.h"
#include "PhCommonUI/TimeCodeEdit.h"

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

public slots:

    void updateText(PhFrame frame, PhTimeCodeType tcType);

private slots:
    void on_actionGoto_triggered();

    void on_actionAbout_triggered();

    void on_actionDocumentation_triggered();

	void on__saveButton_clicked();

	void on__loadButton_clicked();


private:

	void saveSettings();
	void loadSettings();

    Ui::MainWindow *ui;
    PhFrame _frame;
    PhTimeCodeType _tcType;
    GoTo *_goto;
    AboutMenu *_about;
};

#endif // MAINWINDOW_H
