#ifndef PHSTRIPDOC_H
#define PHSTRIPDOC_H


#include <QDebug>
#include <QtXml>

#include "PhTools/PhColor.h"
#include "PhTools/PhTime.h"
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
    PhString _title;
    /**
     * Starting time of the video content refered by the videoPath : String
     */
    PhTimeCode _videoTimestamp;
    /**
     * Path to the video content.
     */
    PhString _videoPath;
    /**
     * Amount of time units per second.
     */
    int _timeScale;
    /**
     * Number of video frame per second. In fact, this attribute can take 4 differents values:
     * 23.98
     * 24
     * 25
     * 29.97
     */
    float _fps;
    /**
     * Specify if the timecode use dropframe or not. In fact, only 29.97 video framerate use dropframe.
     */
    bool _drop;

    /**
     * List of PhPeople from the file
     */
    QMap<PhString, PhPeople *> _actors;

    /**
     * List of PhStripText from the file
     */
    QList<PhStripText *> _texts;

    /**
     * List of PhStripCut form the file
     */
     QList<PhStripCut *> _cuts;


public:
    PhStripDoc(PhString filename);

    PhString getTitle();
    PhTimeCode getVideoTimestamp();
    PhString getVideoPath();
    int getTimeScale();
    float getFps();
    bool getDrop();
    QMap<PhString, PhPeople *> getActors();
    QList<PhStripText *> getTexts();

    void setTitle(PhString _title);
    void setVideoTimestamp(PhTimeCode videoTimestamp);
    void setVideoPath(PhString videoPath);
    void setTimeScale(int timeScale);

    bool openDetX(PhString filename);



};

#endif // PHSTRIPDOC_H
