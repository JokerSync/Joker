#ifndef PHLOCKABLESPINBOXTEST_H
#define PHLOCKABLESPINBOXTEST_H

#include <QObject>

class LockableSpinBoxTest : public QObject
{
	Q_OBJECT
public:
	explicit LockableSpinBoxTest(QObject *parent = 0);

private slots:
	void testMultiClickUnlock();
	void testControlClickUnload();

};

#endif // PHLOCKABLESPINBOXTEST_H
