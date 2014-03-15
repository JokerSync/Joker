#ifndef PHLOCKABLESPINBOXTEST_H
#define PHLOCKABLESPINBOXTEST_H

#include <QObject>

class PhLockableSpinBoxTest : public QObject
{
	Q_OBJECT
public:
	explicit PhLockableSpinBoxTest(QObject *parent = 0);

private slots:
	void testLock();
};

#endif // PHLOCKABLESPINBOXTEST_H
