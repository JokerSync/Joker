#ifndef PHSTRIPCUT_H
#define PHSTRIPCUT_H

#include "PhStrip/PhStripObject.h"

/**
 * A cut is a change in the video from one shot to another. It can be simple (one frame change) or progressive (fade).
 */
class PhStripCut : PhStripObject {

public:
    /**
     * Describes the type of the cut.
     */
    enum PhCutType {
        Simple, FadeIn, FadeOut, CrossFade
    };

    PhStripCut();
    PhStripCut(PhStripCut::PhCutType type);


private:
    PhCutType _type;

};


#endif // PHSTRIPCUT_H
