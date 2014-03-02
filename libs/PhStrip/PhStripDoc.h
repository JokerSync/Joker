/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPDOC_H
#define PHSTRIPDOC_H

#include <QList>
#include <QMap>
#include <QtXml>
#include <QXmlStreamWriter>
#include <QDomDocument>

#include "PhTools/PhTimeCode.h"

#include "PhPeople.h"
#include "PhStripCut.h"
#include "PhStripLoop.h"
#include "PhStripObject.h"
#include "PhStripText.h"
#include "PhStripDetect.h"

/**
 * @brief The joker document class
 *
 * It contains the script file with all the informations
 * such as the title, the authors, the characters (PhPeople), the lines,
 * the attach video file...
 */
class PhStripDoc : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief PhStripDoc
	 * Constructor
	 */
	explicit PhStripDoc(QObject *parent = 0);

	/**
	 * @brief Get the information about the document generator
	 * @return The generator name and version
	 */
	QString getGenerator() {
		return _generator;
	}
	/**
	 * @brief Get the title
	 * @return The title
	 */
	QString getTitle();

	/**
	 * @brief Get the translated title
	 * @return The translated title
	 */
	QString getTranslatedTitle();
	/**
	 * @brief Get the episode
	 * @return the episode
	 */
	QString getEpisode();
	/**
	 * @brief Get the season
	 * @return the season
	 */
	QString getSeason();

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
	 * @brief Get the author name
	 * @return
	 */
	QString getAuthorName() {
		return _authorName;
	}

	/**
	 * @brief Get the list of meta information key
	 * @return A string list
	 */
	QList<QString> getMetaKey();

	/**
	 * @brief Get the meta information of a given key
	 * @param key The meta information key
	 * @return The meta information value
	 */
	QString getMetaInformation(QString key);

	/**
	 * @brief getLastPosition
	 * @return _lastPosition
	 */
	PhFrame getLastFrame();

	/**
	 * @brief getTimeScale
	 * @return
	 */
	int getTimeScale();

	/**
	 * @brief Get the timecode type
	 * @return the corresponding PhTimeCodeType
	 */
	PhTimeCodeType getTCType();

	/**
	 * @brief getActors
	 * @return _actors
	 */
	QMap<QString, PhPeople *> getPeoples();
	/**
	 * @brief Get the list of texts
	 * @return A list of texts
	 */
	QList<PhStripText *> getTexts();

	/**
	 * @brief Get the list of texts affected to a people
	 * @param people The people
	 * @return A list of texts
	 */
	QList<PhStripText *> getTexts(PhPeople *people);

	/**
	 * @brief getLoops
	 * @return _loops
	 */
	QList<PhStripLoop *> getLoops();

	/**
	 * @brief Get the cuts
	 * @return the cut's list
	 */
	QList<PhStripCut *> getCuts();

	/**
	 * @brief Get the detect list
	 * @todo Implement and test frameIn / frameOut
	 * @return A list of PhStripDetect*
	 */
	QList<PhStripDetect *> getDetects(PhFrame frameIn = PHFRAMEMIN, PhFrame frameOut = PHFRAMEMAX);

	/**
	 * @brief Get the list of detect affected to a people in a defined range.
	 * @param people The people
	 * @param frameIn The range in
	 * @param frameOut The range out
	 * @return A list of detects
	 */
	QList<PhStripDetect *> getPeopleDetects(PhPeople *people, PhFrame frameIn = PHFRAMEMIN, PhFrame frameOut = PHFRAMEMAX);

	/**
	 * @brief Set the title property
	 * @param title A string
	 */
	void setTitle(QString title);
	/**
	 * @brief setVideoTimestamp
	 * @param videoFramestamp
	 */
	void setVideoTimestamp(PhFrame videoFramestamp);
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
	 * @brief Open a DetX file
	 * @param filename The path to the DetX file
	 * @return True if the doc opened well, false otherwise
	 */
	bool importDetX(QString filename);
	/**
	 * @brief Open a strip file
	 * @param fileName The path to the DetX file
	 * @return True if the strip opened well, false otherwise
	 */
	bool openStripFile(QString fileName);
	/**
	 * @brief Save the PhStripDoc to a strip file
	 * @param fileName Path to the stripfile
	 * @param lastTC The last displayed timecode
	 * @param forceRatio169 If the aspect ratio has been forced or not.
	 * @return True if the strip saved well, false otherwise
	 */
	bool saveStrip(QString fileName, QString lastTC, bool forceRatio169 = false);
	/**
	 * @brief Create a made up strip using the parameters
	 * @param text The desired text
	 * @param nbPeople The desired number of actors
	 * @param nbText The desired number of sentences
	 * @param nbTrack The desired number of tracks
	 * @param videoTimeCode The starting timecode
	 * @return
	 */
	bool createDoc(QString text, int nbPeople, int nbText, int nbTrack, PhTime videoTimeCode);

	/**
	 * @brief Get the number of texts
	 * Useful for statistics
	 * @return The number of texts from the doc
	 */
	int getNbTexts();

	/**
	 * @brief Get people by their name
	 * @param name The desired people's name
	 * @return The corresponding PhPeople
	 */
	PhPeople * getPeopleByName(QString name);

	/**
	 * @brief Get the next text
	 * @param frame The desired frame
	 * @return The correponding text
	 */
	PhStripText * getNextText(PhFrame frame);

	/**
	 * @brief Get the next text
	 * @param frame The desired frame
	 * @param people The desired PhPeople who speak the texts
	 * @return The correponding text
	 */
	PhStripText * getNextText(PhFrame frame, PhPeople *people);
	/**
	 * @brief Get the next text
	 * @param frame The desired frame
	 * @param peopleList The desired PhPeople list who speak the texts
	 * @return The first corresponding text. If two (or more) texts
	 * have the same timecode, the text attach to the first PhPeople of
	 * the list will be returned
	 */
	PhStripText * getNextText(PhFrame frame, QList<PhPeople*> peopleList);
	/**
	 * @brief Get the previous text frame
	 * @param frame The given frame
	 * @return The frame corresponding to the previous text starting
	 */
	PhFrame getPreviousTextFrame(PhFrame frame);
	/**
	 * @brief Get the previous loop frame
	 * @param frame The given frame
	 * @return The frame corresponding to the previous loop
	 */
	PhFrame getPreviousLoopFrame(PhFrame frame);
	/**
	 * @brief Get the previous cut frame
	 * @param frame the given frame
	 * @return The frame corresponding to the previous cut
	 */
	PhFrame getPreviousCutFrame(PhFrame frame);
	/**
	 * @brief Get previous element frame
	 * @param frame the given frame
	 * @return The frame corresponding to the previous element (cut, loop, text...)
	 */
	PhFrame getPreviousElementFrame(PhFrame frame);
	/**
	 * @brief Get the next text frame
	 * @param frame the given frame
	 * @return The frame corresponding to the next text
	 */
	PhFrame getNextTextFrame(PhFrame frame);
	/**
	 * @brief Get the next loop frame
	 * @param frame the given frame
	 * @return The frame corresponding to the next loop
	 */
	PhFrame getNextLoopFrame(PhFrame frame);
	/**
	 * @brief Get the next cut frame
	 * @param frame the given frame
	 * @return The frame corresponding to the next cut
	 */
	PhFrame getNextCutFrame(PhFrame frame);
	/**
	 * @brief Get the next element frame
	 * @param frame the given frame
	 * @return The frame corresponding to the next element (cut, loop, text...)
	 */
	PhFrame getNextElementFrame(PhFrame frame);
	/**
	 * @brief Get the first frame of the PhStripDoc
	 * @return The frame in
	 */
	PhFrame getFrameIn();
	/**
	 * @brief Get the last frame of the PhStripDoc
	 * @return The frame out
	 */
	PhFrame getFrameOut();
	/*!
	 * \brief Get the force ratio information
	 * \return if the ratio is forced or not
	 */
	bool forceRatio169() const;

	/**
	 * @brief Get the next loop
	 * @param frame the given frame
	 * @return the corresponding loop
	 */
	PhStripLoop * getNextLoop(PhFrame frame);
	/**
	 * @brief Get the previous loop
	 * @param frame the given frame
	 * @return the corresponding loop
	 */
	PhStripLoop * getPreviousLoop(PhFrame frame);

signals:
	/**
	 * @brief Emit a signal when the PhStripDoc changed
	 */
	void changed();

private:
	void reset();

	QString _generator;
	/**
	 * Title of the corresponding audiovisual content.
	 */
	QString _title;
	QString _translatedTitle;
	QString _episode;
	QString _season;
	QMap<QString, QString> _metaInformation;

	/**
	 * Starting time of the video content refered by the videoPath : String
	 */
	PhTime _videoFrameStamp;
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

	PhTimeCodeType _tcType;

	QString _authorName;

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
	QList<PhStripDetect *> _detects;

	int _nbTexts;
	void addText(PhPeople * actor, PhTime start, PhTime end, QString sentence, int track);
	bool _forceRatio169;
};

#endif // PHSTRIPDOC_H
