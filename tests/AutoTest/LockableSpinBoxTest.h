#ifndef PHLOCKABLESPINBOXTEST_H
#define PHLOCKABLESPINBOXTEST_H

#include <QObject>

class LockableSpinBoxTest : public QObject
{
	Q_OBJECT
public:
	explicit LockableSpinBoxTest(QObject *parent = 0);

private slots:
	void testLock();
};

#endif // PHLOCKABLESPINBOXTEST_H
