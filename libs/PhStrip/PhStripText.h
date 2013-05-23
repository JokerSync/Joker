#ifndef PHSTRIPTEXT_H
#define PHSTRIPTEXT_H

#include "phstripdoc.h"
     /**
     * Sentence or part of a sentence of a PhPeople with synchronization information.
     */
class PhStripText : PhStripObject {

private:
    /**
         * Indicate the end of the PhText. The text content is stretch between timeIn : PhTime and timeOut : PhTime
         */
    PhTime timeOut;
    /**
         * Text string of Sentence or part of a sentence.
         */
    PhString content;
};


#endif // PHSTRIPTEXT_H
