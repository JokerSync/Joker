#ifndef PHSTRIPDOC_H
#define PHSTRIPDOC_H


#include <QDebug>

#include <QtXml>

#include "PhTools/PhTime.h"
#include "PhTools/PhTimeCode.h"

#include "PhPeople.h"
#include "PhStripCut.h"
#include "PhStripLoop.h"
#include "PhStripObject.h"
#include "PhStripText.h"

class PhStripDoc {
private:
    void reset();
    /**
     * Title of the corresponding audiovisual content.
     */
	QString _title;
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
	QString _videoPath;
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
	QMap<QString, PhPeople *> _actors;

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
	 void splitText(PhPeople * actor, PhTime start, PhTime end, QString sentence,int track, bool alone, int ite);


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
	QString getTitle();
    /**
     * @brief getVideoTimestamp
     * @return _videoTimestamp
     */
    PhTime getVideoTimestamp();
    /**
     * @brief getVideoPath
     * @return _videoPath
     */
	QString getVideoPath();
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
	QMap<QString, PhPeople *> getActors();
    /**
     * @brief getTexts
     * @return _texts
     */
    QList<PhStripText *> getTexts();

    /**
     * @brief setTitle
     * @param _title
     */
	void setTitle(QString _title);
    /**
     * @brief setVideoTimestamp
     * @param videoTimestamp
     */
    void setVideoTimestamp(PhTimeCode videoTimestamp);
    /**
     * @brief setVideoPath
     * @param videoPath
     */
	void setVideoPath(QString videoPath);
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
	bool openDetX(QString filename);


    int getNbTexts();
};

#endif // PHSTRIPDOC_H
