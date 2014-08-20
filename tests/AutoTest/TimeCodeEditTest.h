#ifndef PHTIMECODEEDITTEST_H
#define PHTIMECODEEDITTEST_H

#include <QObject>

class TimeCodeEditTest : public QObject
{
	Q_OBJECT
public:
	explicit TimeCodeEditTest();

private slots:
	void frameTest();
	void setFrameTest();

	void keyboardInputTest01();
	void keyboardInputTest02();
	void keyboardBadInputTest();

	void mouseInputTest();
};

#endif // PHTIMECODEEDITTEST_H
