#ifndef PHTIMECODEEDITTEST_H
#define PHTIMECODEEDITTEST_H

#include <QObject>

class PhTimeCodeEditTest : public QObject
{
	Q_OBJECT
public:
	explicit PhTimeCodeEditTest(QObject *parent = 0);

private slots:
	void setFrameTest();

};

#endif // PHTIMECODEEDITTEST_H
