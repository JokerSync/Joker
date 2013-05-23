#ifndef PHSTRIPDOC_H
#define PHSTRIPDOC_H

#include "PhTools/phcolor.h"
#include "PhTools/phtime.h"
#include "PhTools/PhTimeCode.h"

#include "PhTools/PhString.h"
#include "PhStrip/PhPeople.h"
#include "PhStrip/PhStripCut.h"
#include "PhStrip/PhStripLoop.h"
#include "PhStrip/PhStripObject.h"
#include "PhStrip/PhStripText.h"

class PhStripDoc {
private:
    /**
     * Title of the corresponding audiovisual content.
     */
    int title;
    /**
     * Starting time of the video content refered by the videoPath : String
     */
    PhTimeCode videoTimestamp;
    /**
     * Path to the video content.
     */
    PhString videoPath;
    /**
     * Amount of time units per second.
     */
    int timeScale;
    /**
     * Number of video frame per second. In fact, this attribute can take 4 differents values:
     * 23.98
     * 24
     * 25
     * 29.97
     */
    float fps;
    /**
     * Specify if the timecode use dropframe or not. In fact, only 29.97 video framerate use dropframe.
     */
    bool drop;

public:
    PhStripDoc();
    void getTitle();
    void setTitle(int title);
    PhTimeCode getVideoTimestamp();
    void setVideoTimestamp(PhTimeCode videoTimestamp);
    PhString getVideoPath();
    void setVideoPath(PhString videoPath);
    int getTimeScale();
    void setTimeScale(int timeScale);

    //TODO
    //bool openDetX(PhString filename);


};





#endif // PHSTRIPDOC_H
