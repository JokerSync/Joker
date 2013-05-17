#include "phdebug.h"
#include "QDebug"

using namespace test;

void PhDebug(QString msg)
{

    qDebug("%d", test::Audio);
    qDebug("%d", test::DoCommand);
    qDebug("%d", test::LTC);
    qDebug("%d", test::LTC);
    qDebug("%x", test::Clock);
    qDebug("%#x", test::Sony);

    qDebug() << "@" + QString::number(LTC) + ": " + msg;
    QtMsgType
}

