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

	virtual void setClock(PhClock *clock);

protected slots:

	virtual void onRateChanged(PhRate rate) = 0;
	virtual void onFrameChanged(PhFrame frame,PhTimeCodeType tcType) = 0;
	virtual void checkVideoPosition() = 0;

protected:
	PhClock *_clock;
};

#endif // PHVIDEOOBJECT_H
