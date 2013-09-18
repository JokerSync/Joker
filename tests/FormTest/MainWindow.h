#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>

#include "PhTools/PhTimeCode.h"
#include "PhCommonUI/TimeCodeEdit.h"

namespace Ui {
class MainWindow;
}

class QDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void updateText(PhFrame frame);

private slots:
    void on_actionGoto_triggered();

private:
    Ui::MainWindow *ui;
    QDialog *_goto;
    PhFrame _frame;
    PhTimeCodeType _tcType;

};

#endif // MAINWINDOW_H
