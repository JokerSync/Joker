#ifndef WINDOWTEST_H
#define WINDOWTEST_H

#include <QObject>

class WindowTest : public QObject
{
	Q_OBJECT
private slots:
	void fullScreenTest01();
	void fullScreenTest02();
};

#endif // WINDOWTEST_H
