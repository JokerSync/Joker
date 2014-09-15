/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QString>
#include <QFileInfo>
#include <QDomDocument>
#include <QDomNodeList>
#include <QtXml>
#include <QXmlStreamWriter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "PhTools/PhDebug.h"
#include "PhTools/PhFileTool.h"

#include "PhStripDoc.h"

PhStripDoc::PhStripDoc()
{
	reset();
}


bool PhStripDoc::importDetXFile(QString fileName)
{
	PHDEBUG << fileName;
	if (!QFile(fileName).exists()) {
		PHDEBUG << "The file doesn't exists" << fileName;
		return false;
	}

	_filePath = fileName;

	// Opening the XML file
	QFile xmlFile(fileName);
	if(!xmlFile.open(QIODevice::ReadOnly)) {
		PHDEBUG << "Unable to open" << fileName;
		return false;
	}

	// Loading the DOM (document object model)
	QDomDocument *domDoc = new QDomDocument();
	if (!domDoc->setContent(&xmlFile)) {
		xmlFile.close();
		PHDEBUG << "The XML document seems to be bad formed " << fileName;
		return false;
	}

	reset();

	QDomElement detX = domDoc->documentElement();

	if(detX.tagName() != "detx") {
		xmlFile.close();
		PHDEBUG << "Bad root element :" << detX.tagName();
		return false;
	}

	_generator = "Cappella";
	//With DetX files, fps is always 25 no drop
	PhTimeCodeType tcType = PhTimeCodeType25;

	// Reading the header
	if(detX.elementsByTagName("header").count()) {
		QDomElement header = detX.elementsByTagName("header").at(0).toElement();

		// Read the Cappella version
		if(header.elementsByTagName("cappella").count())
			_generator += " v" + detX.elementsByTagName("cappella").at(0).toElement().attribute("version");

		// Reading the title
		if(header.elementsByTagName("title").count())
			_title = header.elementsByTagName("title").at(0).toElement().text();
		else
			_title = QFileInfo(fileName).baseName();

		// Reading the translated title
		if(header.elementsByTagName("title2").count())
			_translatedTitle = header.elementsByTagName("title2").at(0).toElement().text();

		// Reading the episode info
		if(header.elementsByTagName("episode").count()) {
			QDomElement episodeElem = header.elementsByTagName("episode").at(0).toElement();
			_episode = episodeElem.attribute("number");
			_season = episodeElem.attribute("season");
		}

		// Reading the video path
		if(header.elementsByTagName("videofile").count()) {
			QDomElement videoFile = header.elementsByTagName("videofile").at(0).toElement();
			_videoPath = videoFile.text();
			// Reading the video time in
			_videoTimeIn = PhTimeCode::timeFromString(videoFile.attribute("timestamp"), tcType);
			_videoTimeCodeType = tcType;
		}

		// Reading the last position
		if(header.elementsByTagName("last_position").count()) {
			QDomElement lastPosition = header.elementsByTagName("last_position").at(0).toElement();
			_lastTime = PhTimeCode::timeFromString(lastPosition.attribute("timecode"), tcType);
		}

		// Reading the author name
		if(header.elementsByTagName("author").count()) {
			QDomElement author = header.elementsByTagName("author").at(0).toElement();
			_authorName = author.attribute("firstname") + " " + author.attribute("name");
		}

		// Reading other meta informations
		if(header.elementsByTagName("production").count()) {
			QDomElement production = header.elementsByTagName("production").at(0).toElement();
			_metaInformation["Producteur"] = production.attribute("producer");
			_metaInformation["Année de production"] = production.attribute("year");
			_metaInformation["Distributeur"] = production.attribute("distributor");
			_metaInformation["Réalisateur"] = production.attribute("director");
			_metaInformation["Diffuseur"] = production.attribute("diffuser");
			_metaInformation["Pays d'origine"] = production.attribute("country");
		}
	}

	QMap<QString, PhPeople*> peopleMap;

	// Reading the "role" lists
	if(detX.elementsByTagName("roles").count()) {
		QDomElement roles = detX.elementsByTagName("roles").at(0).toElement();
		QDomNodeList roleList = roles.elementsByTagName("role");
		for (int i = 0; i < roleList.length(); i++) {
			QDomElement role = roleList.at(i).toElement();
			PhPeople *people = new PhPeople(role.attribute("name"), role.attribute("color"));

			//Currently using id as key instead of name
			peopleMap[role.attribute("id")] = people;
			_peoples.append(people);
		}
	}

	int loopNumber = 1;

	// Reading the strip body
	if(detX.elementsByTagName("body").count()) {
		QDomElement body = detX.elementsByTagName("body").at(0).toElement();
		for(int i = 0; i < body.childNodes().length(); i++) {
			if(body.childNodes().at(i).isElement()) {
				QDomElement elem = body.childNodes().at(i).toElement();

				PhTime timeIn = PhTimeCode::timeFromString(elem.attribute("timecode"), tcType);
				// Reading loops
				if(elem.tagName() == "loop")
					_loops.append(new PhStripLoop(timeIn, QString::number(loopNumber++)));
				// Reading cuts
				else if(elem.tagName() == "shot")
					_cuts.append(new PhStripCut(timeIn, PhStripCut::Simple));
				else if(elem.tagName() == "line") {
					timeIn = -1;
					PhTime lastTime = -1;
					PhTime lastLinkedTime = -1;
					PhPeople *people = peopleMap[elem.attribute("role")];
					float y = elem.attribute("track").toInt() / 4.0;
					QString currentText = "";
					for(int j = 0; j < elem.childNodes().length(); j++) {
						if(elem.childNodes().at(j).isElement()) {
							QDomElement lineElem = elem.childNodes().at(j).toElement();
							if(lineElem.tagName() == "lipsync") {
								lastTime = PhTimeCode::timeFromString(lineElem.attribute("timecode"), tcType);
								if(timeIn < 0)
									timeIn = lastTime;
								if(lineElem.attribute("link") != "off") {
									if(currentText.length()) {
										_texts1.append(new PhStripText(lastLinkedTime, people, lastTime, y, currentText, 0.25f));
										currentText = "";
									}
									lastLinkedTime = lastTime;
								}
							}
							else if(lineElem.tagName() == "text")
								currentText += lineElem.text();
						}
					}
					// Handling line with no lipsync out
					if(currentText.length()) {
						PhTime time = lastLinkedTime + currentText.length() * 1000;
						PHDEBUG << currentText;
						_texts1.append(new PhStripText(lastLinkedTime, people, time, y, currentText, 0.25f));
						lastTime = lastLinkedTime = time;
					}
					PhStripDetect::PhDetectType type = PhStripDetect::On;
					if(elem.attribute("voice") == "off")
						type = PhStripDetect::Off;
					_detects.append(new PhStripDetect(type, timeIn, people, lastTime, y));
				}
			}
		}
	}

	emit this->changed();

	return true;
}

bool PhStripDoc::checkMosTag2(QFile &f, int level, QString expected)
{
	QString name = PhFileTool::readString(f, level, expected);
	if(name != expected) {
		PHDEBUG << "!!!!!!!!!!!!!!!" << "Error reading " << expected << "!!!!!!!!!!!!!!!";
		f.close();
		return false;
	}
	return true;
}

bool PhStripDoc::checkMosTag(QFile &f, int level, MosTag expectedTag)
{
	MosTag tag = readMosTag(f, level, "checkMosTag");

	if(tag != expectedTag) {
		PHDEBUG << "!!!!!!!!!!!!!!!" << "Error reading " << tag << "instead of" << expectedTag << "!!!!!!!!!!!!!!!";
		f.close();
		return false;
	}
	return true;
}

PhTime PhStripDoc::readMosTime(QFile &f, PhTimeCodeType tcType, int level)
{
	return PhFileTool::readInt(f, level, "time") * PhTimeCode::timePerFrame(tcType) / 12;
}

PhStripText* PhStripDoc::readMosText(QFile &f, PhTimeCodeType tcType, int textLevel, int internLevel)
{
	QString content = PhFileTool::readString(f, 2, "content");

	PhTime timeIn = _videoTimeIn + readMosTime(f, tcType, internLevel);
	PhTime timeOut = _videoTimeIn + readMosTime(f, tcType, internLevel);

	PhStripText* text = new PhStripText(timeIn, NULL, timeOut, 0, content, 0.2f);

	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");

	PHDBG(textLevel) << PHNQ(PhTimeCode::stringFromTime(timeIn, tcType))
	                 << "->"
	                 << PHNQ(PhTimeCode::stringFromTime(timeOut, tcType))
	                 << PHNQ(content);
	return text;
}

PhStripDetect *PhStripDoc::readMosDetect(QFile &f, PhTimeCodeType tcType, int detectLevel, int internLevel)
{
	PhTime timeIn = _videoTimeIn + readMosTime(f, tcType, internLevel);
	PhTime timeOut = _videoTimeIn + readMosTime(f, tcType, internLevel);
	PhFileTool::readInt(f, internLevel, "detect type 1");
	int detectType2 = PhFileTool::readInt(f, internLevel, "detect type 2");
	int detectType3 = PhFileTool::readInt(f, internLevel, "detect type 3");
	PhStripDetect::PhDetectType type = PhStripDetect::Unknown;
	switch(detectType3) {
	case 9:
		type = PhStripDetect::SemiOff;
		break;
	case 10:
		type = PhStripDetect::Off;
		break;
	default:
		switch (detectType2) {
		case 0:
			type = PhStripDetect::On;
			break;
		case 2:
			type = PhStripDetect::MouthOpen;
			break;
		case 3:
			type = PhStripDetect::MouthClosed;
			break;
		case 4:
			type = PhStripDetect::Aperture;
			break;
		case 5:
			type = PhStripDetect::Advance;
			break;
		case 6:
			type = PhStripDetect::Labial;
			break;
		case 7:
			type = PhStripDetect::SemiLabial;
			break;
		case 8:
			type = PhStripDetect::Bowl;
			break;
		case 13:
		case 14:
			type = PhStripDetect::Dental;
			break;
		case 15:
			type = PhStripDetect::ArrowUp;
			break;
		case 16:
			type = PhStripDetect::ArrowDown;
			break;
		case 17:
			type = PhStripDetect::AmbianceStart;
			break;
		case 18:
			type = PhStripDetect::AmbianceEnd;
			break;
		}
	}

	for(int j = 0; j < 6; j++)
		PhFileTool::readShort(f, internLevel);
	PHDBG(detectLevel) << "detect: "
	                   << PhTimeCode::stringFromTime(timeIn, tcType)
	                   << PhTimeCode::stringFromTime(timeOut, tcType)
	                   << "type2:"
	                   << detectType2
	                   << "type3:"
	                   << detectType3
	                   << "type:"
	                   << type;
	return new PhStripDetect(type, timeIn, NULL, timeOut, 0);
}

bool PhStripDoc::readMosProperties(QFile &f, int level)
{
	QString originalTitle = PhFileTool::readString(f, level, "Titre de la versio originale");

	QString translatedTitle = PhFileTool::readString(f, level, "Titre de la version adaptée");

	_metaInformation["Titre de la version originale"] = originalTitle;
	_metaInformation["Titre de la version adaptée"] = translatedTitle;

	if(originalTitle.length())
		_title = originalTitle;
	else if(translatedTitle.length())
		_title = translatedTitle;

	_season = PhFileTool::readString(f, level, "Saison");
	_episode = PhFileTool::readString(f, level, "Episode/bobine");
	PhFileTool::readString(f, level, "Titre vo episode");
	PhFileTool::readString(f, level, "Titre adapté de l'épisode");
	PhFileTool::readString(f, level, "Durée");
	PhFileTool::readString(f, level, "Date");
	PhFileTool::readString(f, level, "Client");
	PhFileTool::readString(f, level, "Commentaires");
	PhFileTool::readString(f, level, "Détecteur");
	_authorName = PhFileTool::readString(f, level, "Auteur");
	PhFileTool::readString(f, level, "Studio");
	PhFileTool::readString(f, level, "D.A.");
	PhFileTool::readString(f, level, "Ingénieur du son");

	return true;
}

PhStripDoc::MosTag PhStripDoc::readMosTag(QFile &f, int level, QString name)
{
	unsigned short tag = PhFileTool::readShort(f, level, name);
	if(tag != 0xffff)
		return _mosTagMap[tag];

	PhFileTool::readShort(f, level, name);
	QString stringTag = PhFileTool::readString(f, level, name);

	if(stringTag == "CDocDoublage")
		return _mosTagMap[_mosNextTag++] = MosDub;
	else if(stringTag == "CDocPiste")
		return _mosTagMap[_mosNextTag++] = MosTrack;
	else if(stringTag == "CDocBlocTexte")
		return _mosTagMap[_mosNextTag++] = MosText;
	else if(stringTag == "CDocBlocDetection")
		return _mosTagMap[_mosNextTag++] = MosDetect;
	else if(stringTag == "CDocLangue")
		return _mosTagMap[_mosNextTag++] = MosLang;
	else if(stringTag == "CDocEtiquetteNom")
		return _mosTagMap[_mosNextTag++] = MosLabel;
	else if(stringTag == "CDocBoucle")
		return _mosTagMap[_mosNextTag++] = MosLoop;
	else if(stringTag == "CDocPlan")
		return _mosTagMap[_mosNextTag++] = MosCut;
	else if(stringTag == "CMosaicDoc")
		return _mosTagMap[_mosNextTag++] = MosDoc;
	else if(stringTag == "CDocProjet")
		return _mosTagMap[_mosNextTag++] = MosProject;
	else if(stringTag == "CDocProprietes")
		return _mosTagMap[_mosNextTag++] = MosProperties;
	else if(stringTag == "CDocOptionsProjet")
		return _mosTagMap[_mosNextTag++] = MosOptions;
	else if(stringTag == "CDocFilm")
		return _mosTagMap[_mosNextTag++] = MosMovie;
	else if(stringTag == "CDocPersonnage")
		return _mosTagMap[_mosNextTag++] = MosPeople;
	else if(stringTag == "CDocChutier")
		return _mosTagMap[_mosNextTag++] = MosBin;
	else {
		PHDEBUG << "!!!!!!!!!!!!!!! Unknown tag:" << stringTag << "!!!!!!!!!!!!!!!";
		f.close();
		return MosUnknown;
	}
}

bool PhStripDoc::readMosTrack(QFile &f, PhTimeCodeType tcType, QMap<int, PhPeople *> peopleMap, QMap<int, int> peopleTrackMap, int blocLevel, int textLevel, int detectLevel, int labelLevel, int level, int internLevel)
{
	QList<PhStripDetect*> detectList;
	QList<PhStripText*> textList1, textList2;
	int detectCount = PhFileTool::readInt(f, detectLevel, "track CDocBlocDetection count");

	if(detectCount) {
		if(!checkMosTag(f, blocLevel, MosDetect))
			return false;

		for(int i = 0; i < detectCount; i++) {
			if(i > 0)
				PhFileTool::readShort(f, level, "detect tag");
			detectList.append(readMosDetect(f, tcType, detectLevel, internLevel));
		}
	}

	int langCount = PhFileTool::readInt(f, blocLevel, "track CDocLangue count");
	if(langCount) {
		if(!checkMosTag(f, blocLevel, MosLang))
			return false;
	}

	int textCount = PhFileTool::readInt(f, blocLevel, "track CDocBlocTexte count");
	if(textCount) {
		if(!checkMosTag(f, blocLevel, MosText))
			return false;

		for(int i = 0; i < textCount; i++) {
			if(i > 0)
				PhFileTool::readShort(f, level, "text tag");
			textList1.append(readMosText(f, tcType, textLevel, internLevel));
		}
	}

	int peopleId = PhFileTool::readInt(f, level, "people id");

	for(int k = 0; k < 2; k++) {
		int count = PhFileTool::readInt(f, level, "track other count");
		if(count == 0)
			continue;
		MosTag tag = readMosTag(f, level, "track other tag");
		switch(tag) {
		case MosText:
			for(int i = 0; i < count; i++) {
				if(i > 0)
					PhFileTool::readShort(f, level, "text tag");
				textList2.append(readMosText(f, tcType, textLevel, internLevel));
			}
			break;
		case MosLabel:
			for(int i = 0; i < count; i++) {
				if(i > 0)
					PhFileTool::readShort(f, level, "label tag");
				PhTime labelTime = _videoTimeIn + readMosTime(f, tcType, internLevel);
				for(int j = 0; j < 6; j++)
					PhFileTool::readShort(f, internLevel);
				PHDBG(labelLevel) << "label" << PhTimeCode::stringFromTime(labelTime, tcType);
			}
			break;
		default:
			PHDEBUG << "!!!!!!!!!!!!!!! Unknown tag:" << PHNQ(QString::number(tag, 16)) << "!!!!!!!!!!!!!!!";
			f.close();
			return false;
		}
	}

	PhPeople *people = peopleMap[peopleId];
	int track = peopleTrackMap[peopleId];

	PHDBG(textLevel) << "Adding" << textList1.count() << "texts in list 1";
	foreach(PhStripText* text, textList1) {
		text->setPeople(people);
		text->setY(track / 5.0);
		_texts1.append(text);
	}

	PHDBG(textLevel) << "Adding" << textList2.count() << "texts in  list 2";
	foreach(PhStripText* text, textList2) {
		text->setPeople(people);
		text->setY(track);
		_texts2.append(text);
	}

	foreach(PhStripDetect* detect, detectList) {
		detect->setPeople((people));
		detect->setY(track);
		_detects.append(detect);
	}

	return true;
}
bool PhStripDoc::modified() const
{
	return _modified;
}

void PhStripDoc::setModified(bool modified)
{
	_modified = modified;
}


bool PhStripDoc::importMosFile(const QString &fileName)
{
	PHDEBUG << "===============" << fileName << "===============";

	QFile f(fileName);
	if(!f.exists()) {
		PHDEBUG << "File doesn't exists : " << fileName;
		return false;
	}

	if(!f.open(QFile::ReadOnly)) {
		PHDEBUG << "Unable to open : " << fileName;
		return false;
	}

	this->reset();

	_filePath = fileName;
	_title = QFileInfo(fileName).baseName();

	int level = 1;
	int ok = 0;
	int propLevel = ok;
	int peopleLevel = level;
	int textLevel = level;
	int detectLevel = level;
	int blocLevel = level;
	int cutLevel = level;
	int loopLevel = level;
	int labelLevel = level;
	int internLevel = 2;

	if(!checkMosTag2(f, blocLevel, "NOBLURMOSAIC"))
		return false;

	_generator = "Mosaic";

	PhFileTool::readShort(f, blocLevel, "CMosaicDoc");
	PhFileTool::readShort(f, blocLevel, "CMosaicDoc");

	if(!checkMosTag2(f, blocLevel, "CMosaicDoc"))
		return false;

	PhFileTool::readShort(f, blocLevel, "CDocProjet");
	PhFileTool::readShort(f, blocLevel, "CDocProjet");

	if(!checkMosTag2(f, blocLevel, "CDocProjet"))
		return false;

	PhFileTool::readShort(f, blocLevel, "CDocProprietes");
	PhFileTool::readShort(f, blocLevel, "CDocProprietes");

	if(!checkMosTag2(f, blocLevel, "CDocProprietes"))
		return false;

	readMosProperties(f, propLevel);

	PhFileTool::readShort(f, blocLevel, "CDocOptionsProjet");

	// read a number that makes a difference wether it's 3 or 4 later
	unsigned short mosVersion = PhFileTool::readShort(f, blocLevel, "CDocOptionsProjet mosVersion");


	if(!checkMosTag2(f, blocLevel, "CDocOptionsProjet"))
		return false;

	PhTimeCodeType tcType;
	unsigned short type = PhFileTool::readInt(f, level, "type");
	bool drop = PhFileTool::readInt(f, level, "drop") != 0;
	switch(type) {
	case 0:
		if(drop)
			tcType = PhTimeCodeType2398;
		else
			tcType = PhTimeCodeType24;
		break;
	default:
		tcType = PhTimeCodeType25;
		break;
	}
	PHDBG(ok) << "TC Type:" << tcType;


	if(mosVersion == 4) {
		//		qDebug() << "reading extrasection ???";
		//		PhFileTool::readInt(f, logLevel, "loop continuous numbering");
		PhFileTool::readShort(f, level);
		PhFileTool::readShort(f, level);
	}

	for(int j = 0; j < 8; j++)
		PhFileTool::readShort(f, level);

	PhFileTool::readInt(f, blocLevel, "CDocFilm count");
	PhFileTool::readShort(f, blocLevel, "CDocFilm");
	PhFileTool::readShort(f, blocLevel, "CDocFilm");

	if(!checkMosTag2(f, blocLevel, "CDocFilm"))
		return false;

	unsigned short peopleCount = PhFileTool::readInt(f, blocLevel, "CDocPersonnage count");

	PhFileTool::readShort(f, blocLevel, "CDocPersonnage");
	int peopleType = PhFileTool::readShort(f, blocLevel, "CDocPersonnage");

	if(!checkMosTag2(f, blocLevel, "CDocPersonnage"))
		return false;

	QMap<int, PhPeople*> peopleMap;
	QMap<int, int> peopleTrackMap;
	for(int i = 0; i < peopleCount; i++) {
		if(i > 0)
			PhFileTool::readShort(f, level, "people tag");

		int peopleId = PhFileTool::readInt(f, peopleLevel, "peopleId");

		QString name = PhFileTool::readString(f, peopleLevel, "people name");
		PhPeople *people = new PhPeople(name, "#000000");
		peopleMap[peopleId] = people;
		_peoples.append(people);

		peopleTrackMap[peopleId] = PhFileTool::readInt(f, peopleLevel, "people track") - 1;
		for(int j = 0; j < 6; j++)
			PhFileTool::readShort(f, level);


		if(peopleType == 2)
			PhFileTool::readString(f, peopleLevel, "date 1");
	}

	int peopleCount2 = PhFileTool::readInt(f, blocLevel, "people count 2");
	if(peopleCount2 != peopleCount) {
		PHDEBUG << "people count not corresponding:" << peopleCount << "/" << peopleCount2;
		//		return false;
	}
	PhFileTool::readShort(f, blocLevel, "CDocVideo");
	unsigned short videoType = PhFileTool::readShort(f, blocLevel, "CDocVideo");

	if(!checkMosTag2(f, blocLevel, "CDocVideo"))
		return false;

	QString videoFilePath = PhFileTool::readString(f, ok, "Video path");
	this->setVideoFilePath(videoFilePath);
	PhTime videoTimeIn = readMosTime(f, tcType, internLevel);
	this->setVideoTimeIn(videoTimeIn, tcType);
	PHDBG(ok) << "Timestamp:" << PhTimeCode::stringFromTime(_videoTimeIn, tcType);

	if(videoType == 3) {
		PhFileTool::readShort(f, level, "videoType3");
		PhFileTool::readShort(f, level, "videoType3");
	}

	PhFileTool::readShort(f, level);
	PhFileTool::readShort(f, level);

	unsigned short cutCount = PhFileTool::readInt(f, blocLevel, "cut count");
	if(cutCount) {
		if(!checkMosTag(f, blocLevel, MosCut))
			return false;

		for(int j = 0; j < cutCount; j++) {
			if((j > 0) && !checkMosTag(f, level, MosCut))
				return false;
			PhTime cutTime = _videoTimeIn + readMosTime(f, tcType, internLevel);
			PHDBG(cutLevel) << "cut:" << PhTimeCode::stringFromTime(cutTime, tcType);
			_cuts.append(new PhStripCut(cutTime, PhStripCut::Simple));
		}
	}

	QString script = PhFileTool::readString(f, ok, "script");

	PhFileTool::readInt(f, blocLevel, "dub count");
	if(!checkMosTag(f, blocLevel, MosDub))
		return false;

	for(int j = 0; j < 8; j++)
		PhFileTool::readShort(f, level, "CDocDoublage");

	int trackCount = PhFileTool::readInt(f, blocLevel, "track count");
	if(!checkMosTag(f, blocLevel, MosTrack))
		return false;

	for(int track = 0; track < trackCount; track++) {
		PHDBG(level) << "====== READING TRACK " << track << "======";
		if((track > 0) && !checkMosTag(f, level, MosTrack))
			return false;
		if(!readMosTrack(f, tcType, peopleMap, peopleTrackMap, blocLevel, textLevel, detectLevel, labelLevel, level, internLevel))
			return false;
	}

	PHDBG(level) << "====== END OF TRACK ======";

	for(int k = 0; k < 2; k++) {
		int loopCount = PhFileTool::readInt(f, loopLevel, "loop count");
		if(loopCount == 0)
			continue;
		if(!checkMosTag(f, blocLevel, MosLoop))
			return false;
		for(int i = 0; i < loopCount; i++) {
			if((i > 0) && !checkMosTag(f, level, MosLoop))
				return false;
			int number = PhFileTool::readInt(f, loopLevel, "loop number");

			PhTime loopTime = _videoTimeIn + readMosTime(f, tcType, internLevel);
			QString label = PhFileTool::readString(f, loopLevel, "loop name");
			if(k == 1)
				label = "off";
			else if(label.isEmpty())
				label = QString::number(number);
			_loops.append(new PhStripLoop(loopTime, label));
		}
	}

	for(int j = 0; j < 4; j++)
		PhFileTool::readShort(f, level, "after loop1");

	//	if(strangeNumber2 == 1) {
	//		for(int j = 0; j < 9; j++)
	//			PhFileTool::readShort(f, level, "after loop2");
	//	}

	//	if(!checkMosTag(f, blocLevel, MosBin))
	//		return false;

	//	for(int j = 0; j < 2; j++)
	//		PhFileTool::readShort(f, level);

	PHDEBUG << "_______________" << "reading ok" << "_______________";

	f.close();

	if((_texts1.count() == 0) && (_texts2.count())) {
		PHDEBUG << "Switching primary and secondary text lists";
		_texts1.append(_texts2);
		_texts2.clear();
	}

	qSort(_texts1.begin(), _texts1.end(), PhStripObject::dtcomp);
	qSort(_texts2.begin(), _texts2.end(), PhStripObject::dtcomp);
	qSort(_detects.begin(), _detects.end(), PhStripObject::dtcomp);
	qSort(_cuts.begin(), _cuts.end(), PhStripObject::dtcomp);
	qSort(_loops.begin(), _loops.end(), PhStripObject::dtcomp);

	emit this->changed();

	return true;
}

PhTime PhStripDoc::ComputeDrbTime1(PhTime offset, PhTime value, PhTimeCodeType tcType)
{
	return (offset + value) * PhTimeCode::timePerFrame(tcType) / 400000;
}

PhTime PhStripDoc::ComputeDrbTime2(PhTime offset, PhTime value, PhTimeCodeType tcType)
{
	return (offset + value * 50000) * PhTimeCode::timePerFrame(tcType) / 400000;
}

bool PhStripDoc::importDrbFile(const QString &fileName)
{
	PHDEBUG << fileName;
	QFile file(fileName);

	reset();

#warning /// @todo Handle more drb frame rate
	PhTimeCodeType tcType = PhTimeCodeType25;

	PhTime offset = 0;

	if(file.open(QIODevice::ReadOnly)) {
		QTextStream ts(&file);
		// Detect text codec
		if(file.peek(2).at(1) == 0)
			ts.setCodec("UTF-16");

		while(!ts.atEnd()) {
			QString line = ts.readLine();
			if(line.startsWith("CV:")) {
				_videoPath = line.remove("CV:");
				PHDEBUG << "videoPath:" << _videoPath;
			}
			else if(line.startsWith("O:")) {
				offset = line.remove("O:").toLongLong();
				_videoTimeIn = ComputeDrbTime1(offset, 0, tcType);
				PHDEBUG << "videoTimeIn:" << _videoTimeIn << line;
			}
		}
	}
	else {
		PHDEBUG << "Unable to open:" << fileName;
		return false;
	}

	QString dirName = fileName;
	dirName.remove(".drb", Qt::CaseInsensitive);

	QFile loopFile(dirName + "/boucle.xml");
	if(!loopFile.open(QIODevice::ReadOnly)) {
		PHDEBUG << "Unable to open boucle.xml";
		return false;
	}

	QDomDocument loopDoc;
	if(!loopDoc.setContent(&loopFile)) {
		loopFile.close();
		PHDEBUG << "Unable to parse boucle.xml";
		return false;
	}

	int loopNumber = 1;
	QDomNodeList loopList = loopDoc.elementsByTagName("Row");
	for (int i = 0; i < loopList.length(); i++) {
		QDomElement loopElement = loopList.at(i).toElement();
		QString type = loopElement.elementsByTagName("Type").at(0).toElement().text();
		PhTime timeIn = ComputeDrbTime1(offset, loopElement.elementsByTagName("Debut").at(0).toElement().text().toLongLong(), tcType);
		if(type == "BOUCLE") {
			_loops.append(new PhStripLoop(timeIn, QString::number(loopNumber++)));
		}
		else if (type == "PLAN") {
			_cuts.append(new PhStripCut(timeIn, PhStripCut::PhCutType::Simple));
		}
	}

	loopFile.close();

	// Opening the XML file
	QFile peopleFile(dirName + "/intervenant.xml");
	if(!peopleFile.open(QIODevice::ReadOnly)) {
		PHDEBUG << "Unable to open intervenant.xml";
		return false;
	}

	// Loading the DOM (document object model)
	QDomDocument peopleDoc;
	if (!peopleDoc.setContent(&peopleFile)) {
		peopleFile.close();
		PHDEBUG << "The XML document seems to be bad formed intervenant.xml";
		return false;
	}

	bool result = true;
	QMap<int, PhPeople*> peopleMap;

	QDomNodeList peopleList = peopleDoc.elementsByTagName("Row");
	for (int i = 0; i < peopleList.length(); i++) {
		QDomElement peopleElement = peopleList.at(i).toElement();
		int id = peopleElement.elementsByTagName("Id").at(0).toElement().text().toInt();
		QString name = peopleElement.elementsByTagName("Nom").at(0).toElement().text();
		PhPeople *people = new PhPeople(name);
		peopleMap[id] = people;
	}

	peopleFile.close();

	foreach(PhPeople *people, peopleMap.values())
		_peoples.append(people);

	QDir dir(dirName);

	foreach(QString name, dir.entryList(QStringList("*.dat"))) {
		QString subFileName = dir.filePath(name);
		QFile f(subFileName);
		if(f.open(QIODevice::ReadOnly)) {
			QTextStream ts(&f);

			// Detect text codec
			if(f.peek(2).at(1) == 0)
				ts.setCodec("UTF-16");

			QString xmlString = "";

			while(!ts.atEnd()) {
				QString line = ts.readLine();
				if(!line.startsWith("<COPYRIGHT"))
					xmlString += line + "\n";
				if(line == "</SYNCHRONOS>")
					break;
			}
			f.close();

			QDomDocument subDoc;

			QString errorMsg;
			int errorLine, errorColumn;
			if(subDoc.setContent(xmlString, &errorMsg, &errorLine, &errorColumn)) {
				QDomNodeList textList = subDoc.elementsByTagName("TEXT");
				for(int i = 0; i < textList.count(); i++) {
					QDomElement textElement = textList.at(i).toElement();
					int peopleId = textElement.elementsByTagName("ID_INTER").at(0).toElement().text().toInt();
					PhPeople *people = peopleMap[peopleId];
					PhTime timeIn = ComputeDrbTime2(offset, textElement.elementsByTagName("X1").at(0).toElement().text().toLongLong() - 150, tcType);
					PhTime timeOut = ComputeDrbTime2(offset, textElement.elementsByTagName("X2").at(0).toElement().text().toLongLong() - 150, tcType);
					int y1 = textElement.elementsByTagName("Y1").at(0).toElement().text().toInt();
					int y2 = textElement.elementsByTagName("Y2").at(0).toElement().text().toInt();
#warning /// @todo make sure 150 is the maximum Y value:
					float y = y1 / 150.0f;
					float height = (y2 - y1) / 150.0f;

					QString content = textElement.elementsByTagName("VALUE").at(0).toElement().text();

					PHDEBUG << PhTimeCode::stringFromTime(timeIn, tcType) << PhTimeCode::stringFromTime(timeOut, tcType) << content;
					PhStripText *text = new PhStripText(timeIn, people, timeOut, y, content, height);
					_texts1.append(text);
				}
			}
			else {
				PHDEBUG << "Unable to parse" << subFileName << ":" << errorMsg << "@" << errorLine << "," << errorColumn;
				result = false;
			}
		}
		else {
			PHDEBUG << "Unable to open" << subFileName;
			result = false;
		}
	}

	return result;
}

bool PhStripDoc::importSyn6File(const QString &fileName)
{
	QSqlDatabase db;
	db =  QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(fileName);
	if(!db.open()) {
		PHDEBUG << "Error opening the sqlite document:" << db.lastError().text();
		return false;
	}
	PHDEBUG << "database opened: " << db.tables().count() << "tables.";

//	foreach(QString tableName, db.tables()) {
//		PHDEBUG << tableName;
//	}

	QSqlQuery query(db);

	PhTimeCodeType tcType = PhTimeCodeType25;

	// Reading video file and timestamp
	PhTime offset = 0;
	if(query.exec("SELECT * FROM PREFERENCE;")) {
		PHDEBUG << "PREFERENCE:";
		while(query.next()) {
//			for(int i = 0; i < 7; i++)
//				PHDEBUG << i << query.value(i);
			switch(query.value(0).toInt()) {
			case 6:
				offset = query.value(3).toLongLong();
				_videoTimeIn = ComputeDrbTime1(offset, 0, tcType);
				break;
			case 11:
				_videoPath = query.value(2).toString().replace("\\\\", "\\");
				break;
			}
		}
	}

	// Reading peoples
	QMap<int, PhPeople*> peopleMap;
	if(query.exec("SELECT * FROM PERSONNAGE;")) {
		PHDEBUG << "PERSONNAGE:";

		while(query.next()) {
			int id = query.value(0).toInt();
			QString name = query.value(1).toString();
			PhPeople *people = new PhPeople(name);
			peopleMap[id] = people;
		}
	}
	else
		PHDEBUG << "query failed";

	foreach(PhPeople *people, peopleMap.values())
		_peoples.append(people);

	// Reading loops
	if(query.exec("SELECT * FROM OBJET_TC;")) {
		PHDEBUG << "OBJET_TC:";
		while(query.next()) {
			for(int i = 0; i < 7; i++)
				PHDEBUG << i << query.value(i);
			PhTime time = ComputeDrbTime2(offset, query.value(2).toLongLong(), tcType);
			switch(query.value(1).toInt()) {
			case 2:
				_cuts.append(new PhStripCut(time, PhStripCut::Simple));
				break;
			case 7:
				_loops.append(new PhStripLoop(time, QString::number(query.value(4).toInt())));
				break;
			}
		}
	}

	// Reading texts
	if(query.exec("SELECT * FROM TEXTE;")) {
		PHDEBUG << "TEXTE:";
		while(query.next()) {
//			for(int i = 0; i < 21; i++)
//				PHDEBUG << i << query.value(i);
#warning /// @todo check text people id
			PhPeople* people = peopleMap[query.value(0).toInt()];
#warning /// @todo check text time in/out
			PhTime timeIn = ComputeDrbTime2(offset, query.value(3).toLongLong() - 150, tcType);
			PhTime timeOut = ComputeDrbTime2(offset, query.value(4).toLongLong() - 150, tcType);
			int y1 = query.value(6).toInt();
#warning /// @todo make sure y2 is at the index 6
			int y2 = query.value(5).toInt();
#warning /// @todo make sure 150 is the maximum Y value:
			float y = y1 / 150.0f;
			float height = (y2 - y1) / 150.0f;
			QString content = query.value(7).toString();
			PhStripText *text = new PhStripText(timeIn, people, timeOut, y, content, height);
			_texts1.append(text);
			PHDEBUG << timeIn << timeOut << content;
		}
	}

	db.close();

	return true;
}

bool PhStripDoc::openStripFile(const QString &fileName)
{
	PHDEBUG << fileName;
	bool result = false;

	QString extension = QFileInfo(fileName).suffix().toLower();
	// Try to open the document
	if(extension == "detx") {
		return importDetXFile(fileName);
	}
	else if(extension == "mos") {
		return importMosFile(fileName);
	}
	else if(extension == "drb") {
		return importDrbFile(fileName);
	}
	else if(extension == "syn6") {
		return importSyn6File(fileName);
	}
	else if(extension == "strip" or extension == "joker") {
		QFile xmlFile(fileName);
		if(!xmlFile.open(QIODevice::ReadOnly)) {
			PHDEBUG << "Unable to open" << fileName << xmlFile.errorString();
			return false;
		}

		// Loading the DOM
		QDomDocument *domDoc = new QDomDocument();
		if (!domDoc->setContent(&xmlFile)) {
			xmlFile.close();
			PHDEBUG << "The XML document seems to be bad formed" << fileName;
			return false;
		}
		xmlFile.close();

		PHDEBUG << ("Start parsing " + fileName);
		QDomElement stripDocument = domDoc->documentElement();

		if(stripDocument.tagName() != "strip" && stripDocument.tagName() != "joker") {
			PHDEBUG << "Bad root element :" << stripDocument.tagName();
			return false;
		}

		result = true;

		// Reading the media nodes
		QDomNodeList mediaList = stripDocument.elementsByTagName("media");
		for(int i = 0; i < mediaList.count(); i++) {
			QDomElement media = mediaList.at(i).toElement();
			QString type = media.attribute("type");
			PHDEBUG << "line" << type;
			if(type == "detx")
				result = importDetXFile(media.text());
			else if(type == "mos")
				result = importMosFile(media.text());
			else if(type == "video") {
				_videoPath = media.text();

				float fps = media.attribute("frameRate").toFloat();
				_videoTimeCodeType = PhTimeCode::computeTimeCodeType(fps);
				_videoTimeIn = PhTimeCode::timeFromString(media.attribute("tcStamp"), _videoTimeCodeType);

				_videoForceRatio169 = media.attribute("forceRatio").toLower() == "yes";
				_videoDeinterlace = media.attribute("deinterlace").toLower() == "yes";
			}
		}

		if(stripDocument.elementsByTagName("state").count()) {
			QDomElement state = stripDocument.elementsByTagName("state").at(0).toElement();
			_lastTime = state.attribute("lastTime").toLongLong();
			if(_lastTime == 0)
				_lastTime = PhTimeCode::timeFromString(state.attribute("lastTimeCode"), _videoTimeCodeType);
		}

		if(stripDocument.elementsByTagName("peoples").count()) {
			QDomNodeList chars = stripDocument.elementsByTagName("peoples").at(0).childNodes();
			for(int i = 0; i < chars.count(); i++) {
				QString color = chars.at(i).toElement().attribute("color");
				QString name = chars.at(i).toElement().attribute("name");
				peopleByName(name)->setColor(color);
			}
		}

	}
	return result;
}

bool PhStripDoc::saveStripFile(const QString &fileName, PhTime lastTime)
{
	PHDEBUG << fileName;
	QFile file(fileName);

	// open a file
	if (!file.open(QIODevice::WriteOnly)) {
		PHDEBUG << "an error occur while saving the strip document";
		return false;
	}
	else {
		//if file is successfully opened then create XML
		QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
		// set device (here file)to streamwriter
		xmlWriter->setDevice(&file);
		// Writes a document start with the XML version number version.

		// Positive numbers indicate spaces, negative numbers tabs.
		xmlWriter->setAutoFormattingIndent(-1);
		xmlWriter->setAutoFormatting(true);

		// Indent is just for keeping in mind XML structure
		xmlWriter->writeStartDocument();
		xmlWriter->writeStartElement("joker");
		{
			xmlWriter->writeStartElement("meta");
			{
				xmlWriter->writeStartElement("generator");
				xmlWriter->writeAttribute("name", "Joker");
#ifdef APP_VERSION
				xmlWriter->writeAttribute("version", APP_VERSION);
#endif
				xmlWriter->writeEndElement();

				if(_filePath.length() > 0) {
					QFileInfo info(_filePath);
					xmlWriter->writeStartElement("media");
					xmlWriter->writeAttribute("type", info.suffix());
					xmlWriter->writeCharacters(filePath());
					xmlWriter->writeEndElement();
				}

				xmlWriter->writeStartElement("media");
				xmlWriter->writeAttribute("type", "video");
				xmlWriter->writeAttribute("tcStamp", PhTimeCode::stringFromTime(_videoTimeIn, _videoTimeCodeType));
				xmlWriter->writeAttribute("frameRate", QString::number(PhTimeCode::getAverageFps(_videoTimeCodeType)));

				if(_videoForceRatio169)
					xmlWriter->writeAttribute("forceRatio", "yes");
				if(_videoDeinterlace)
					xmlWriter->writeAttribute("deinterlace", "yes");
				xmlWriter->writeCharacters(_videoPath);
				xmlWriter->writeEndElement();

				xmlWriter->writeStartElement("state");
				xmlWriter->writeAttribute("lastTime", QString::number(lastTime));
				xmlWriter->writeEndElement();
			}
			xmlWriter->writeEndElement();

			xmlWriter->writeStartElement("peoples");
			{
				foreach(PhPeople * ppl, peoples()) {
					xmlWriter->writeStartElement("people");
					xmlWriter->writeAttribute("name", ppl->name());
					xmlWriter->writeAttribute("color", ppl->color());
					xmlWriter->writeEndElement();

				}
			}
			xmlWriter->writeEndElement();

		}
		xmlWriter->writeEndElement();

		xmlWriter->writeEndDocument();
		delete xmlWriter;
	}

	return true;
}

void PhStripDoc::generate(QString content, int loopCount, int peopleCount, PhTime spaceBetweenText, int textCount, int trackCount, PhTime videoTimeIn)
{
	this->reset();
	_title = "Generate file";
	_translatedTitle = "Fichier généré";
	_episode = "1";
	_season = "1";
	_videoTimeIn = videoTimeIn;
	_videoTimeCodeType = PhTimeCodeType25;
	_lastTime = _videoTimeIn;

	if (trackCount > 4 || trackCount < 1)
		trackCount = 3;

	QStringList names;
	names.append("Actor");
	names.append("Actress");
	names.append("Jack");
	names.append("Jane");

	int nbNames = names.length();
	// Creation of the Peoples
	for (int i = 1; i <= peopleCount; i++) {
		PhPeople *people = new PhPeople(names.at(i % nbNames) + " " + QString::number(i), "black");
		_peoples.append(people);
	}

	PhTime time = _videoTimeIn;
	// Creation of the text
	for (int i = 0; i < textCount; i++) {
		//Make people "talk" alternaly
		PhPeople *people = _peoples[i % peopleCount];

		PhTime timeIn = time;
		PhTime timeOut = timeIn + content.length() * 1000;

		_texts1.append(new PhStripText(timeIn, people, timeOut, i % trackCount / 4, content, 0.25f));

		// So the texts are all one after the other
		time += spaceBetweenText;
	}

	// Add a loop per minute
	for(int i = 0; i < loopCount; i++)
		_loops.append(new PhStripLoop(_videoTimeIn + i * 24000 * 60, QString::number(i)));

	emit changed();
}

void PhStripDoc::reset()
{
	_peoples.clear();
	_cuts.clear();
	_detects.clear();
	_lastTime = 0;
	_loops.clear();
	_texts1.clear();
	_texts2.clear();
	_title = "";
	_translatedTitle = "";
	_episode = "";
	_season = "";
	_videoPath = "";
	_videoTimeIn = 0;
	_videoTimeCodeType = PhTimeCodeType25;
	_videoDeinterlace = false;
	_authorName = "";
	_videoForceRatio169 = false;
	_generator = "";
	_mosNextTag = 0x8008;
	_modified = false;

	emit this->changed();
}

void PhStripDoc::addObject(PhStripObject *object)
{
	if(dynamic_cast<PhStripCut*>(object)) {
		this->_cuts.append(dynamic_cast<PhStripCut*>(object));
		PHDEBUG << "Added a cut";
	}
	else if(dynamic_cast<PhStripLoop*>(object)) {
		this->_loops.append(dynamic_cast<PhStripLoop*>(object));
		PHDEBUG << "Added a loop";
	}
	else if(dynamic_cast<PhStripDetect*>(object)) {
		this->_detects.append(dynamic_cast<PhStripDetect*>(object));
		PHDEBUG << "Added a detect!";
	}
	else if(dynamic_cast<PhStripText*>(object)) {
		this->_texts1.append(dynamic_cast<PhStripText*>(object));
		PHDEBUG << "Added a text!";
	}
	else {
		PHDEBUG << "You try to add a weird object, which seems to be undefined...";
	}
	emit changed();

}

void PhStripDoc::addPeople(PhPeople *people)
{
	this->_peoples.append(people);
	PHDEBUG << "Added a people";
	emit changed();

}

PhPeople *PhStripDoc::peopleByName(QString name)
{
	foreach(PhPeople* people, _peoples) {
		if(people && people->name() == name)
			return people;
	}
	return NULL;
}

PhStripText *PhStripDoc::nextText(PhTime time)
{
	PhStripText * result = NULL;
	foreach(PhStripText* text, this->texts()) {
		if(text->timeIn() > time) {
			if(!result || (text->timeIn() < result->timeIn()) )
				result = text;
		}
	}
	return result;
}

PhStripText *PhStripDoc::nextText(PhPeople *people, PhTime time)
{
	PhStripText * result = NULL;
	foreach(PhStripText* text, this->texts()) {
		if((text->people() == people) && (text->timeIn() > time)) {
			if(!result || (text->timeIn() < result->timeIn()) )
				result = text;
		}
	}
	return result;
}

PhStripText *PhStripDoc::nextText(QList<PhPeople *> peopleList, PhTime time)
{
	PhStripText * result = NULL;
	foreach(PhStripText* text, this->texts()) {
		if(peopleList.contains(text->people()) && (text->timeIn() > time)) {
			if(!result || (text->timeIn() < result->timeIn()) )
				result = text;
		}
	}
	return result;
}

PhTime PhStripDoc::previousTextTime(PhTime time)
{
	PhTime previousTextTime = PHTIMEMIN;

	foreach(PhStripText* text, this->texts()) {
		if((text->timeIn() < time) && (text->timeIn() > previousTextTime) )
			previousTextTime = text->timeIn();
	}

	return previousTextTime;
}

PhTime PhStripDoc::previousLoopTime(PhTime time)
{
	PhTime previousLoopTime = PHTIMEMIN;

	foreach(PhStripLoop* loop, _loops) {
		if((loop->timeIn() < time) && (loop->timeIn() > previousLoopTime) )
			previousLoopTime = loop->timeIn();
	}

	return previousLoopTime;
}

PhTime PhStripDoc::previousCutTime(PhTime time)
{
	PhTime previousCutTime = PHTIMEMIN;

	foreach(PhStripCut* cut, _cuts) {
		if((cut->timeIn() < time) && (cut->timeIn() > previousCutTime) )
			previousCutTime = cut->timeIn();
	}

	return previousCutTime;
}

PhTime PhStripDoc::previousElementTime(PhTime time)
{
	PhTime previousElementTime = previousCutTime(time);

	if(previousLoopTime(time) > previousElementTime)
		previousElementTime = previousLoopTime(time);

	if(previousTextTime(time) > previousElementTime)
		previousElementTime = previousTextTime(time);

	return previousElementTime;
}

PhTime PhStripDoc::nextTextTime(PhTime time)
{
	PhTime nextTextTime = PHTIMEMAX;

	foreach(PhStripText* text, this->texts()) {
		if((text->timeIn() > time) && (text->timeIn() < nextTextTime) )
			nextTextTime = text->timeIn();
		else if(text->timeIn() > nextTextTime)
			return nextTextTime;
	}

	return nextTextTime;
}

PhTime PhStripDoc::nextLoopTime(PhTime time)
{
	PhTime nextLoopTime = PHTIMEMAX;

	foreach(PhStripLoop* loop, _loops) {
		if((loop->timeIn() > time) && (loop->timeIn() < nextLoopTime) )
			nextLoopTime = loop->timeIn();
		else if(loop->timeIn() > nextLoopTime)
			return nextLoopTime;
	}

	return nextLoopTime;
}

PhTime PhStripDoc::nextCutTime(PhTime time)
{
	PhTime nextCutTime = PHTIMEMAX;

	foreach(PhStripCut* cut, _cuts) {
		if((cut->timeIn() > time) && (cut->timeIn() < nextCutTime) )
			nextCutTime = cut->timeIn();
		else if(cut->timeIn() > nextCutTime)
			return nextCutTime;
	}

	return nextCutTime;
}

PhTime PhStripDoc::nextElementTime(PhTime time)
{
	PhTime nextElementTime = nextCutTime(time);

	if(nextLoopTime(time) < nextElementTime)
		nextElementTime = nextLoopTime(time);

	if(nextTextTime(time) < nextElementTime)
		nextElementTime = nextTextTime(time);

	return nextElementTime;
}

PhTime PhStripDoc::timeIn()
{
	return nextElementTime(0);
}

PhTime PhStripDoc::timeOut()
{
	return previousElementTime(PHTIMEMAX);
}

PhStripLoop *PhStripDoc::nextLoop(PhTime time)
{
	foreach(PhStripLoop* loop, _loops) {
		if(loop->timeIn() > time)
			return loop;
	}
	return NULL;
}

PhStripLoop *PhStripDoc::previousLoop(PhTime time)
{
	int i = _loops.count() - 1;
	while(i >= 0) {
		if(_loops.at(i)->timeIn() < time)
			return _loops.at(i);
		i--;
	}
	return NULL;
}

QString PhStripDoc::filePath()
{
	return _filePath;
}

QString PhStripDoc::videoFilePath()
{
	return _videoPath;
}

QList<QString> PhStripDoc::metaKeys()
{
	return _metaInformation.keys();
}

QString PhStripDoc::metaInformation(QString key)
{
	return _metaInformation[key];
}

QList<PhPeople *> PhStripDoc::peoples()
{
	return _peoples;
}

QString PhStripDoc::title()
{
	return _title;
}

QString PhStripDoc::translatedTitle()
{
	return _translatedTitle;
}

QString PhStripDoc::episode()
{
	return _episode;
}

QString PhStripDoc::season()
{
	return _season;
}

PhTimeCodeType PhStripDoc::videoTimeCodeType()
{
	return _videoTimeCodeType;
}

PhTime PhStripDoc::videoTimeIn()
{
	return _videoTimeIn;
}

PhTime PhStripDoc::videoFrameIn()
{
	return _videoTimeIn / PhTimeCode::timePerFrame(_videoTimeCodeType);
}

PhTime PhStripDoc::lastTime()
{
	return _lastTime;
}

void PhStripDoc::setForceRatio169(bool forceRatio)
{
	_videoForceRatio169 = forceRatio;
}

bool PhStripDoc::forceRatio169() const
{
	return _videoForceRatio169;
}

QList<PhStripText *> PhStripDoc::texts(bool alternate)
{
	if(alternate)
		return _texts2;
	else
		return _texts1;
}

QList<PhStripText *> PhStripDoc::texts(PhPeople *people)
{
	QList<PhStripText*> result;
	foreach(PhStripText *text, this->texts()) {
		if(text->people() == people)
			result.append(text);
	}
	return result;
}

QList<PhStripLoop *> PhStripDoc::loops()
{
	return _loops;
}

QList<PhStripDetect *> PhStripDoc::detects(PhTime timeIn, PhTime timeOut)
{
	QList<PhStripDetect*> result;
	foreach(PhStripDetect *detect, this->_detects) {
		if((detect->timeIn() >= timeIn) && (detect->timeOut() < timeOut))
			result.append(detect);
	}

	return result;
}

QList<PhStripDetect *> PhStripDoc::peopleDetects(PhPeople *people, PhTime timeIn, PhTime timeOut)
{
	QList<PhStripDetect *> result;
	foreach(PhStripDetect *detect, this->detects(timeIn, timeOut)) {
		if(detect->people() == people)
			result.append(detect);
	}
	return result;
}

void PhStripDoc::setTitle(QString title)
{
	_title = title;
}

void PhStripDoc::setVideoFilePath(QString filePath)
{
	_videoPath = filePath;
}

void PhStripDoc::setVideoTimeIn(PhTime timeIn, PhTimeCodeType tcType)
{
	_videoTimeIn = timeIn;
	_videoTimeCodeType = tcType;
}

QList<PhStripCut *> PhStripDoc::cuts()
{
	return _cuts;
}

