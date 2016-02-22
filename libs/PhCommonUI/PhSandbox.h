#ifndef PHSANDBOX_H
#define PHSANDBOX_H

#include <QByteArray>

QByteArray createSecurityScopeBookmark(QString fileName);

void resolveSecurityScopeBookmark(QString fileName, QByteArray bookmark);

void stopSecurityScopeBookmark(QString fileName, QByteArray bookmark);

#endif // PHSANDBOX_H
