#ifndef TIMECODEINPUT_H
#define TIMECODEINPUT_H

#include <QWidget>
#include <QDebug>

#include "PhTools/PhTimeCode.h"


namespace Ui {
class TimeCodeInput;
}

class TimeCodeInput : public QWidget
{
    Q_OBJECT

public:
    explicit TimeCodeInput(QWidget *parent = 0);
    ~TimeCodeInput();

    void setTCType(PhTimeCodeType tcType);

public slots:

    void isTimeCode();


signals:

    void frameChanged(PhFrame frame);

private:

    Ui::TimeCodeInput *ui;
    PhFrame _frame;
    PhTimeCodeType _tcType;


};

#endif // TIMECODEINPUT_H
