#ifndef PHSTRIPCUT_H
#define PHSTRIPCUT_H

#include "PhStrip/PhStripObject.h"

/**
 * A cut is a change in the video from one shot to another. It can be simple (one frame change) or progressive (fade).
 */
class PhStripCut : PhStripObject {

public:

    PhStripCut();
    PhStripCut(PhStripCut::CrossFade type);
    /**
     * Describes the type of the cut.
     */
    enum PhCutType {
        Simple, FadeIn, FadeOut, CrossFade
    };


private:
    PhCutType _type;

};


#endif // PHSTRIPCUT_H
