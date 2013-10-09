#ifndef PHSTRIPDOC_H
#define PHSTRIPDOC_H

#include <QList>
#include <QMap>

#include "PhTools/PhTimeCode.h"

#include "PhPeople.h"
#include "PhStripCut.h"
#include "PhStripLoop.h"
#include "PhStripObject.h"
#include "PhStripText.h"
#include "PhStripOff.h"

class PhStripDoc : public QObject
{
	Q_OBJECT

public:
     /**
     * @brief PhStripDoc
     * @param filename
     * Constructor
     */
    explicit PhStripDoc(QObject *parent = 0);

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
	 * @brief getFilePath
	 * @return _filePath
	 */
	QString getFilePath();

    /**
     * @brief getVideoPath
     * @return _videoPath
     */
	QString getVideoPath();
    /**
     * @brief getLastPosition
     * @return _lastPosition
     */
    PhTime getLastFrame();
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

    PhTimeCodeType getTCType();

	/**
     * @brief getActors
     * @return _actors
     */
	QMap<QString, PhPeople *> getPeoples();
    /**
     * @brief getTexts
     * @return _texts
     */
    QList<PhStripText *> getTexts();

	/**
	 * @brief getLoops
	 * @return _loops
	 */
	QList<PhStripLoop *> getLoops();

	/**
	 * @brief getOffs
	 * @return _offs
	 */
	QList<PhStripOff *> getOffs();

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

	// First version : Create StripDoc for testing purposes
	bool createDoc(QString text, int nbPeople, int nbLoop, int nbText, int nbTrack, PhTime videoTimeCode);

    int getNbTexts();

	PhFrame getPreviousTextFrame(PhFrame frame);

	PhFrame getPreviousLoopFrame(PhFrame frame);

	PhFrame getPreviousCutFrame(PhFrame frame);

	PhFrame getPreviousElementFrame(PhFrame frame);

	PhFrame getNextTextFrame(PhFrame frame);

	PhFrame getNextLoopFrame(PhFrame frame);

	PhFrame getNextCutFrame(PhFrame frame);

	PhFrame getNextElementFrame(PhFrame frame);

signals:
	void changed();

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
    PhTime _lastFrame;

	/**
	 * Path to the file content.
	 */
	QString _filePath;

    /**
     * Path to the video content.
     */
	QString _videoPath;
    /**
     * Amount of time units per second.
     */
    int _timeScale;

	PhTimeCodeType _tcType;

    /**
     * List of PhPeople from the file
     */
	QMap<QString, PhPeople *> _peoples;

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

	 /**
	  * List of PhStripOff from the file
	  */
	 QList<PhStripOff *> _offs;

     int _nbTexts;
	 void splitText(PhPeople * actor, PhTime start, PhTime end, QString sentence,int track, int ite);
};

#endif // PHSTRIPDOC_H
