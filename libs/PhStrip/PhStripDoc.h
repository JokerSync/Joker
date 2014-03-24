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
	explicit PhStripDoc(QObject *parent = 0);

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
	 * @brief The first frame of the video file
	 * @return A frame value
	 */
	PhFrame videoFramestamp();

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
	 * @brief getLastPosition
	 * @return _lastPosition
	 */
	PhFrame lastFrame();

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
	QList<PhStripText *> texts();

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
	 * @todo Implement and test frameIn / frameOut
	 * @return A list of detects
	 */
	QList<PhStripDetect *> detects(PhFrame frameIn = PHFRAMEMIN, PhFrame frameOut = PHFRAMEMAX);

	/**
	 * @brief Get the list of detect affected to a people in a defined range.
	 * @param people The people
	 * @param frameIn The range in
	 * @param frameOut The range out
	 * @return A list of detects
	 */
	QList<PhStripDetect *> peopleDetects(PhPeople *people, PhFrame frameIn = PHFRAMEMIN, PhFrame frameOut = PHFRAMEMAX);

	/**
	 * @brief Set the title property
	 * @param title A string
	 */
	void setTitle(QString title);
	/**
	 * @brief Set the video first frame
	 * @param A PhFrame
	 */
	void setVideoFramestamp(PhFrame videoFramestamp);
	/**
	 * @brief Set the video file path
	 * @param videoFilePath A string
	 */
	void setVideoFilePath(QString videoFilePath);
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
	bool importMosFile(QString fileName);
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
	bool saveStripFile(QString fileName, QString lastTC, bool forceRatio169 = false);
	/**
	 * @brief Create a made up strip using the parameters
	 * @param text The desired text
	 * @param nbPeople The desired number of actors
	 * @param nbText The desired number of sentences
	 * @param nbTrack The desired number of tracks
	 * @param videoFramestamp The starting frame
	 * @return
	 */
	bool create(QString text, int nbPeople, int nbText, int nbTrack, PhFrame videoFramestamp);

	/**
	 * @brief Get people by their name
	 * @param name The desired people's name
	 * @return The corresponding PhPeople
	 */
	PhPeople * peopleByName(QString name);

	/**
	 * @brief Get the next text
	 * @param frame The desired frame
	 * @return The correponding text
	 */
	PhStripText * nextText(PhFrame frame);

	/**
	 * @brief Get the next text
	 * @param frame The desired frame
	 * @param people The desired PhPeople who speak the texts
	 * @return The correponding text
	 */
	PhStripText * nextText(PhFrame frame, PhPeople *people);
	/**
	 * @brief Get the next text
	 * @param frame The desired frame
	 * @param peopleList The desired PhPeople list who speak the texts
	 * @return The first corresponding text. If two (or more) texts
	 * have the same frameIn, the text attach to the first PhPeople of
	 * the list will be returned
	 */
	PhStripText * nextText(PhFrame frame, QList<PhPeople*> peopleList);
	/**
	 * @brief Get the previous text frame
	 * @param frame The given frame
	 * @return The frame corresponding to the previous text starting
	 */
	PhFrame previousTextFrame(PhFrame frame);
	/**
	 * @brief Get the previous loop frame
	 * @param frame The given frame
	 * @return The frame corresponding to the previous loop
	 */
	PhFrame previousLoopFrame(PhFrame frame);
	/**
	 * @brief Get the previous cut frame
	 * @param frame the given frame
	 * @return The frame corresponding to the previous cut
	 */
	PhFrame previousCutFrame(PhFrame frame);
	/**
	 * @brief Get previous element frame
	 * @param frame the given frame
	 * @return The frame corresponding to the previous element (cut, loop, text...)
	 */
	PhFrame previousElementFrame(PhFrame frame);
	/**
	 * @brief Get the next text frame
	 * @param frame the given frame
	 * @return The frame corresponding to the next text
	 */
	PhFrame nextTextFrame(PhFrame frame);
	/**
	 * @brief Get the next loop frame
	 * @param frame the given frame
	 * @return The frame corresponding to the next loop
	 */
	PhFrame nextLoopFrame(PhFrame frame);
	/**
	 * @brief Get the next cut frame
	 * @param frame the given frame
	 * @return The frame corresponding to the next cut
	 */
	PhFrame nextCutFrame(PhFrame frame);
	/**
	 * @brief Get the next element frame
	 * @param frame the given frame
	 * @return The frame corresponding to the next element (cut, loop, text...)
	 */
	PhFrame nextElementFrame(PhFrame frame);
	/**
	 * @brief Get the first frame of the PhStripDoc
	 * @return The frame in
	 */
	PhFrame frameIn();
	/**
	 * @brief Get the last frame of the PhStripDoc
	 * @return The frame out
	 */
	PhFrame frameOut();
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
	PhStripLoop * nextLoop(PhFrame frame);
	/**
	 * @brief Get the previous loop
	 * @param frame the given frame
	 * @return the corresponding loop
	 */
	PhStripLoop * previousLoop(PhFrame frame);

	/**
	 * @brief Reset the document
	 */
	void reset();

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
	 * Starting frame of the video content refered by the videoPath : String
	 */
	PhFrame _videoFrameStamp;
	/**
	 * @brief The last position
	 */
	PhFrame _lastFrame;

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
	QList<PhPeople *> _peoples;

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

	void addText(PhPeople * actor, PhFrame frameIn, PhFrame frameOut, QString sentence, int track);

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
	PhStripText *readMosText(QFile &f, int level);
	void readMosDetect(QFile &f, int level);
	bool readMosProperties(QFile &f, int level);
	MosTag readMosTag(QFile &f, int level, QString name);
	bool readMosTrack(QFile &f, QMap<int, PhPeople*> peopleMap, QMap<int, int> peopleTrackMap, int blocLevel, int textLevel, int detectLevel, int labelLevel, int level);
	bool _forceRatio169;
};

#endif // PHSTRIPDOC_H
