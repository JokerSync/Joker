#ifndef PHSTRIPLOOP_H
#define PHSTRIPLOOP_H

#include "phstripdoc.h"

/**
 * Indicate the start of a new loop. A loop is in fact a section of the rythmo strip. The detection process consists to cut the video content into loops to simplify the convocation of the dubbing actors for recording. It is generally between 30 seconds and 1 minute long: what the actor can record in one take.
 */
class PhStripLoop : public PhStripObject {

private:
/**
 * Number of the loop. The first loop can be 1 or another value. The loop numbering is usually continuous but not always.
 */
    int _number;

public:
    PhStripLoop();
    PhStripLoop(int number);
};


#endif // PHSTRIPLOOP_H
