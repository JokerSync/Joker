/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPDOC_H
#define PHSTRIPDOC_H

#include <QList>
#include <QMap>
#include <QFile>

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
	 * @brief PhStripDoc constructor
	 */
	PhStripDoc();

	/**
	 * @brief The name of the application that generated the document
	 * @return A string
	 */
	QString generator() {
		return _generator;
	}
	/**
	 * @brief The title
	 * @return A string
	 */
	QString title();

	/**
	 * @brief The translated title
	 * @return A string
	 */
	QString translatedTitle();
	/**
	 * @brief Get the episode
	 * @return A string
	 */
	QString episode();
	/**
	 * @brief Get the season
	 * @return A string
	 */
	QString season();

	/**
	 * @brief The video starting time
	 * @return A time value
	 */
	PhTime videoTimeIn();

	/**
	 * @brief The video starting frame
	 * @return A frame value
	 */
	PhTime videoFrameIn();

	/**
	 * @brief getFilePath
	 * @return _filePath
	 */
	QString filePath();

	/**
	 * @brief getVideoPath
	 * @return _videoPath
	 */
	QString videoFilePath();

	/**
	 * @brief Check if video shall be deinterlace
	 * @return True if deinterlace false otherwise
	 */
	bool videoDeinterlace() {
		return _videoDeinterlace;
	}

	/**
	 * @brief Get the author name
	 * @return
	 */
	QString authorName() {
		return _authorName;
	}

	/**
	 * @brief Get the list of meta information key
	 * @return A string list
	 */
	QList<QString> metaKeys();

	/**
	 * @brief Get the meta information of a given key
	 * @param key The meta information key
	 * @return The meta information value
	 */
	QString metaInformation(QString key);

	/**
	 * @brief The document ending time
	 * @return A time value.
	 */
	PhTime timeOut();

	/**
	 * @brief getTimeScale
	 * @return
	 */
	int timeScale();

	/**
	 * @brief The timecode type
	 * @return A PhTimeCodeType value
	 */
	PhTimeCodeType timeCodeType();

	/**
	 * @brief The list of peoples
	 * @return A list.
	 */
	QList<PhPeople *> peoples();
	/**
	 * @brief The whole text list
	 * @return A list of texts
	 */
	QList<PhStripText *> texts(bool alternate = false);

	/**
	 * @brief The list of texts affected to a people
	 * @param people A people
	 * @return A list of texts
	 */
	QList<PhStripText *> texts(PhPeople *people);

	/**
	 * @brief The whole loop list
	 * @return A list of loops
	 */
	QList<PhStripLoop *> loops();

	/**
	 * @brief The whole cut list
	 * @return A list of cut
	 */
	QList<PhStripCut *> cuts();

	/**
	 * @brief The whole detect list
	 * @todo Implement and test timeIn / timeOut
	 * @return A list of detects
	 */
	QList<PhStripDetect *> detects(PhTime timeIn = PHTIMEMIN, PhTime timeOut = PHTIMEMAX);

	/**
	 * @brief Get the list of detect affected to a people in a defined range.
	 * @param people The people
	 * @param timeIn The range starting time
	 * @param timeOut The range ending time
	 * @return A list of detects
	 */
	QList<PhStripDetect *> peopleDetects(PhPeople *people, PhTime timeIn = PHTIMEMIN, PhTime timeOut = PHTIMEMAX);

	/**
	 * @brief Set the title property
	 * @param title A string
	 */
	void setTitle(QString title);
	/**
	 * @brief Set the video starting time
	 * @param time A time value
	 */
	void setVideoTimeIn(PhTime time);
	/**
	 * @brief Set the video starting frame
	 * @param frame A frame value
	 */
	void setVideoFrameIn(PhFrame frame);
	/**
	 * @brief Set the video file path
	 * @param videoFilePath A string
	 */
	void setVideoFilePath(QString videoFilePath);

	/**
	 * @brief Set the video deinterlace mode
	 * @param deinterlace True if deinterlace false otherwise
	 */
	void setVideoDeinterlace(bool deinterlace) {
		_videoDeinterlace = deinterlace;
	}

	/**
	 * @brief setTimeScale
	 * @param timeScale
	 */
	void setTimeScale(int timeScale);
	/**
	 * @brief Import a DetX file
	 * @param fileName The path to the DetX file
	 * @return True if the doc opened well, false otherwise
	 */
	bool importDetXFile(QString fileName);
	/**
	 * @brief Import a Mos file
	 * @param fileName The path to the Mos file
	 * @return True if the doc opened well, false otherwise
	 */
	bool importMosFile(const QString &fileName);
	/**
	 * @brief Open a strip file
	 * @param fileName The path to the DetX file
	 * @return True if the strip opened well, false otherwise
	 */
	bool openStripFile(const QString &fileName);
	/**
	 * @brief Save the PhStripDoc to a strip file
	 * @param fileName Path to the stripfile
	 * @param lastTC The last displayed timecode
	 * @return True if the strip saved well, false otherwise
	 */
	bool saveStripFile(const QString &fileName, const QString &lastTC);
	/**
	 * @brief Generate a document from given value
	 * @param text The desired text
	 * @param loopCount The number of loops
	 * @param peopleCount The desired number of actors
	 * @param spaceBetweenText Time between two texts
	 * @param textCount The desired number of sentences
	 * @param trackCount The desired number of tracks
	 * @param videoTimeIn The starting time of the document
	 */
	void generate(QString text, int loopCount, int peopleCount, PhTime spaceBetweenText, int textCount, int trackCount, PhTime videoTimeIn);

	/**
	 * @brief Get people by their name
	 * @param name The desired people's name
	 * @return The corresponding PhPeople
	 */
	PhPeople * peopleByName(QString name);

	/**
	 * @brief Get the next text after a time value
	 * @param time A time value
	 * @return The next text or NULL if no text after the time value
	 */
	PhStripText * nextText(PhTime time);

	/**
	 * @brief Get the next text affected to a people after a time value
	 * @param people A people reference
	 * @param time A time value
	 * @return The next text or NULL if no text after the time value
	 */
	PhStripText * nextText(PhPeople *people, PhTime time);
	/**
	 * @brief Get the next text affected to one of a people list after a time value
	 * @param peopleList A people list
	 * @param time A time value
	 * @return The first corresponding text. If two (or more) texts
	 * have the same timeIn, the text attach to the first PhPeople of
	 * the list will be returned
	 */
	PhStripText * nextText(QList<PhPeople*> peopleList, PhTime time);
	/**
	 * @brief Get the previous text before a time value
	 * @param time A time value
	 * @return A time value
	 */
	PhTime previousTextTime(PhTime time);
	/**
	 * @brief Get the previous loop time
	 * @param time A time value
	 * @return A time value
	 */
	PhTime previousLoopTime(PhTime time);
	/**
	 * @brief Get the previous cut time
	 * @param time A time value
	 * @return A time value
	 */
	PhTime previousCutTime(PhTime time);
	/**
	 * @brief Get previous element time
	 * @param time A time value
	 * @return A time value
	 */
	PhTime previousElementTime(PhTime time);
	/**
	 * @brief Get the next text time
	 * @param time A time value
	 * @return A time value
	 */
	PhTime nextTextTime(PhTime time);
	/**
	 * @brief Get the next loop time
	 * @param time A time value
	 * @return A time value
	 */
	PhTime nextLoopTime(PhTime time);
	/**
	 * @brief Get the next cut time
	 * @param time A time value
	 * @return A time value
	 */
	PhTime nextCutTime(PhTime time);
	/**
	 * @brief Get the next element time
	 * @param time A time value
	 * @return A time value
	 */
	PhTime nextElementTime(PhTime time);
	/**
	 * @brief Get the starting time of the document
	 * @return time A time value
	 */
	PhTime timeIn();
	/**
	 * @brief Get the last position the document was edited.
	 * @return time A time value
	 */
	PhTime lastTime();

	/**
	 * @brief Set the force 16/9 ratio status
	 * @param forceRatio A bool value
	 */
	void setForceRatio169(bool forceRatio);

	/**
	 * @brief Get the force 16/9ratio status
	 * @return True if the ratio is forced, false otherwise
	 */
	bool forceRatio169() const;

	/**
	 * @brief Get the next loop
	 * @param time A time value
	 * @return the corresponding loop
	 */
	PhStripLoop * nextLoop(PhTime time);
	/**
	 * @brief Get the previous loop
	 * @param time A time value
	 * @return the corresponding loop
	 */
	PhStripLoop * previousLoop(PhTime time);

	/**
	 * @brief Reset the document
	 */
	void reset();

	/**
	 * @brief Add a PhGraphicObjet to the doc
	 */
	void addObject(PhStripObject *object);
	/**
	 * @brief Add a PhPeople to the doc
	 * @param people the new poeple
	 */
	void addPeople(PhPeople * people);

	/**
	 * @brief modified
	 * @return true if the PhStripDoc have been modified, false otherwise
	 */
	bool modified() const;
	/**
	 * @brief setModified
	 * @param modified
	 */
	void setModified(bool modified);

signals:
	/**
	 * @brief Emit a signal when the PhStripDoc changed
	 */
	void changed();

private:


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
	 * Starting time of the video content refered by the videoPath
	 */
	PhTime _videoTimeIn;
	/**
	 * @brief The last position the document was edited.
	 */
	PhTime _lastTime;

	/**
	 * Path to the file content.
	 */
	QString _filePath;

	/**
	 * Path to the video content.
	 */
	QString _videoPath;
	bool _videoDeinterlace;

	PhTimeCodeType _tcType;

	QString _authorName;

	/**
	 * List of PhPeople from the file
	 */
	QList<PhPeople *> _peoples;

	QList<PhStripText *> _texts1, _texts2;

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

	enum MosTag {
		MosUnknown,
		MosDub,
		MosTrack,
		MosLang,
		MosPeople,
		MosText,
		MosDetect,
		MosLabel,
		MosLoop,
		MosCut,
		MosVideo,
		MosMovie,
		MosOptions,
		MosDoc,
		MosProject,
		MosProperties,
		MosBin,
	};
	unsigned short _mosNextTag;
	QMap<unsigned short, MosTag> _mosTagMap;

	bool checkMosTag2(QFile &f, int level, QString expected);
	bool checkMosTag(QFile &f, int level, MosTag expectedTag);
	PhTime readMosTime(QFile &f, PhTimeCodeType tcType, int level);
	PhStripText *readMosText(QFile &f, int textLevel, int internLevel);
	PhStripDetect *readMosDetect(QFile &f, int detectLevel, int internLevel);
	bool readMosProperties(QFile &f, int level);
	MosTag readMosTag(QFile &f, int level, QString name);
	bool readMosTrack(QFile &f, QMap<int, PhPeople*> peopleMap, QMap<int, int> peopleTrackMap, int blocLevel, int textLevel, int detectLevel, int labelLevel, int level, int internLevel);
	bool _videoForceRatio169;
	bool _modified;
};

#endif // PHSTRIPDOC_H
