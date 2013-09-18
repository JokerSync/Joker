#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

#include "PhCommonUI/TimecodeEdit.h"
#include "PhTools/PhTimeCode.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:

    void frameChanged(PhFrame frame);

private:
    Ui::Dialog *ui;
    //TimecodeEdit _timecodeEdit;

};

#endif // DIALOG_H
