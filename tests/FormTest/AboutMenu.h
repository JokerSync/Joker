#ifndef ABOUTMENU_H
#define ABOUTMENU_H

#include <QDialog>

namespace Ui {
class AboutMenu;
}

class AboutMenu : public QDialog
{
    Q_OBJECT

public:
    explicit AboutMenu(QWidget *parent = 0);
    ~AboutMenu();

private:
    Ui::AboutMenu *ui;
};

#endif // ABOUTMENU_H
