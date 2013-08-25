#ifndef PHGRAPHICCONTROLLER_H
#define PHGRAPHICCONTROLLER_H

#include <QSettings>
#include <QDebug>
#include <QDir>
#include <QObject>

#include "PhStrip/PhStripDoc.h"

class PhGraphicController : public QObject
{
    Q_OBJECT

public:
    explicit PhGraphicController(QObject *parent = 0);


};

#endif // PHGRAPHICCONTROLLER_H
