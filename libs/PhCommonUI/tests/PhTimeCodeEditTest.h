#ifndef PHTIMECODEEDITTEST_H
#define PHTIMECODEEDITTEST_H

#include <QObject>

class PhTimeCodeEditTest : public QObject
{
	Q_OBJECT
public:
	explicit PhTimeCodeEditTest();

private slots:
	void frameTest();
	void setFrameTest();

	void keyboardInputTest();
	void keyboardBadInputTest();

	void mouseInputTest();
};

#endif // PHTIMECODEEDITTEST_H
