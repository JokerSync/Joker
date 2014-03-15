#ifndef PHLOCKABLESPINBOX_H
#define PHLOCKABLESPINBOX_H

#include <QSpinBox>
#include <QTime>

class PhLockableSpinBox : public QSpinBox
{
	Q_OBJECT
public:
	explicit PhLockableSpinBox(QWidget *parent = 0);

protected:
	bool eventFilter(QObject *sender, QEvent *event);

private:
	int _pressCounter;
	QTime _pressTimer;
};

#endif // PHLOCKABLESPINBOX_H
