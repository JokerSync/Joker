/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHSTRIPCUT_H
#define PHSTRIPCUT_H

#include "PhStrip/PhStripObject.h"

/**
 * A cut is a change in the video from one shot to another. It can be simple (one frame change) or progressive (fade).
 */
class PhStripCut : public PhStripObject {

public:
    /**
     * Describes the type of the cut.
     */
    enum PhCutType {
        Simple, FadeIn, FadeOut, CrossFade
    };

    /**
     * @brief PhStripCut
     * @param type
     * @param time
     */
    PhStripCut(PhStripCut::PhCutType type, PhTime time);


private:
    /**
     * Type of cut.
     */
    PhCutType _type;

};


#endif // PHSTRIPCUT_H
