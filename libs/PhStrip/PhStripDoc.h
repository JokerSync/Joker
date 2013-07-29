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

#include "PhTools/PhColor.cpp"
#include "PhTools/PhTime.cpp"
#include "PhTools/PhTimeCode.cpp"
#include "PhTools/PhString.cpp"

#include "PhStrip/PhPeople.cpp"
#include "PhStrip/PhStripCut.cpp"
#include "PhStrip/PhStripLoop.cpp"
#include "PhStrip/PhStripObject.cpp"
#include "PhStrip/PhStripText.cpp"



class PhStripDoc {
private:
    void reset();
    /**
     * Title of the corresponding audiovisual content.
     */
    PhString _title;
    /**
     * Starting time of the video content refered by the videoPath : String
     */
    PhTime _videoTimestamp;
    /**
     * @brief _lastPosition
     */
    PhTime _lastPosition;
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

     /**
      * List of PhStripLoop from the file
      */
     QList<PhStripLoop *> _loops;
     int _nbTexts;
     void splitText(PhPeople * actor, PhTime start, PhTime end, PhString sentence,int track, bool alone, int ite);


public:
     /**
     * @brief PhStripDoc
     * @param filename
     * Constructor
     */
    PhStripDoc();

    QList<PhStripCut *> getCuts();

    /**
     * @brief getTitle
     * @return _title
     */
    PhString getTitle();
    /**
     * @brief getVideoTimestamp
     * @return _videoTimestamp
     */
    PhTime getVideoTimestamp();
    /**
     * @brief getVideoPath
     * @return _videoPath
     */
    PhString getVideoPath();
    /**
     * @brief getLastPosition
     * @return _lastPosition
     */
    PhTime getLastPosition();
    /**
     * @brief getDuration
     * @return duration
     * used to know the script lenght, not very accurate for the moment
     */
    int getDuration(); //TODO Delete me
    /**
     * @brief getTimeScale
     * @return
     */
    int getTimeScale();
    /**
     * @brief getFps
     * @return _fps
     */
    float getFps();
    /**
     * @brief getDrop
     * @return _drop
     */
    bool getDrop();
    /**
     * @brief getActors
     * @return _actors
     */
    QMap<PhString, PhPeople *> getActors();
    /**
     * @brief getTexts
     * @return _texts
     */
    QList<PhStripText *> getTexts();

    /**
     * @brief setTitle
     * @param _title
     */
    void setTitle(PhString _title);
    /**
     * @brief setVideoTimestamp
     * @param videoTimestamp
     */
    void setVideoTimestamp(PhTimeCode videoTimestamp);
    /**
     * @brief setVideoPath
     * @param videoPath
     */
    void setVideoPath(PhString videoPath);
    /**
     * @brief setTimeScale
     * @param timeScale
     */
    void setTimeScale(int timeScale);
    /**
     * @brief openDetX
     * @param filename
     * @return
     */
    bool openDetX(PhString filename);


    int getNbTexts();
};

#endif // PHSTRIPDOC_H
