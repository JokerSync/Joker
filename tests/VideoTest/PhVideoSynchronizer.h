/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHVIDEOSYNCHRONIZER_H
#define PHVIDEOSYNCHRONIZER_H

#include <QObject>

#include "PhTools/PhClock.h"

class PhVideoSynchronizer : public QObject
{
    Q_OBJECT
public:
    PhVideoSynchronizer();

    void setVideoClock(PhClock *clock);
    void setInternalClock(PhClock * clock);

private slots:
    void onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType);
    void onVideoRateChanged(PhRate rate);
    void onInternalFrameChanged(PhFrame frame, PhTimeCodeType tcType);
    void onInternalRateChanged(PhRate rate);
private:

    PhClock * _internalClock;
    PhClock * _videoClock;
};
#endif // PHVIDEOSYNCHRONIZER_H
