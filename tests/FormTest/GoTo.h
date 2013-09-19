#ifndef GOTO_H
#define GOTO_H

#include <QDialog>

#include "PhCommonUI/TimecodeEdit.h"
#include "PhTools/PhTimeCode.h"

namespace Ui {
class GoTo;
}

class GoTo : public QDialog
{
    Q_OBJECT

public:
    explicit GoTo(QWidget *parent = 0);
    ~GoTo();

signals:
    void frameChanged(PhFrame frame, PhTimeCodeType tcType);

private:
    Ui::GoTo *ui;

};

#endif // GOTO_H
