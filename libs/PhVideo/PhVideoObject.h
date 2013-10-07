#ifndef PHVIDEOOBJECT_H
#define PHVIDEOOBJECT_H

#include <PhTools/PhClock.h>

class PhVideoObject
{
public:
	PhVideoObject();

	/**
	 * @brief Open a video file
	 * @param fileName A video file path
	 * @return True if the file was opened successfully, false otherwise
	 */
	virtual bool open(QString fileName) = 0;

	PhClock* getClock(){ return &_clock; };

	void setFrameStamp(PhFrame frame);

	PhFrame frameStamp() { return _frameStamp;};


protected slots:

	virtual void onRateChanged(PhRate rate) = 0;
	virtual void onFrameChanged(PhFrame frame,PhTimeCodeType tcType) = 0;
	virtual void checkVideoPosition() = 0;

protected:
	PhClock _clock;
	PhFrame _frameStamp;
};

#endif // PHVIDEOOBJECT_H
