#ifndef PHMEDIAPANEL_H
#define PHMEDIAPANEL_H

#include <QWidget>
#include <QDebug>

#include "PhTools/PhTimeCode.h"

namespace Ui {
	class PhMediaPanel;
}

class PhMediaPanel : public QWidget
{
	Q_OBJECT
	
public:
	explicit PhMediaPanel(QWidget *parent = 0);
	~PhMediaPanel();
	void setTCType(PhTimeCodeType tcType);
	PhTimeCodeType getTCType() const;
	void setFirstFrame(PhFrame firstFrame);
	PhFrame getFirstFrame() const;
	void setMediaLength(PhFrame mediaLength);
	PhFrame getMediaLength();

signals:

	void playButtonSignal();
	void forwardButtonSignal();
	void rewindButtonSignal();
	void backButtonSignal();
	void nextFrameButtonSignal();
	void previousFrameButtonSignal();
	void useSliderCursorSignal(); // TODO add parameter
	void tcTypeChanged(PhTimeCodeType tcType);

public slots:
	void onFrameChanged(PhFrame frame);
	void onRateChanged(PhRate rate);
	void selectRate();

private slots:

private:

	Ui::PhMediaPanel *ui;
	PhTimeCodeType _tcType;
	PhFrame _firstFrame;
	PhFrame _mediaLength;//number of frames of the file

};

#endif // PHMEDIAPANEL_H
