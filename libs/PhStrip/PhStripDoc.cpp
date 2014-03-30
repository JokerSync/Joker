/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include <QFileInfo>
#include <QDomDocument>
#include <QDomNodeList>
#include <QtXml>
#include <QXmlStreamWriter>

#include "PhTools/PhFileTool.h"
#include "PhStripDoc.h"

PhStripDoc::PhStripDoc(QObject *parent) :
	QObject(parent)
{
	reset();
}


bool PhStripDoc::importDetXFile(QString fileName)
{
	//	PHDEBUG << fileName;
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
	_tcType = PhTimeCodeType25;

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
			_videoTimeIn = PhTimeCode::timeFromString(videoFile.attribute("timestamp"), _tcType);
		}

		// Reading the last position
		if(header.elementsByTagName("last_position").count()) {
			QDomElement lastPosition = header.elementsByTagName("last_position").at(0).toElement();
			_lastTime = PhTimeCode::timeFromString(lastPosition.attribute("timecode"), _tcType);
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

				// Reading loops
				if(elem.tagName() == "loop")
					_loops.append(new PhStripLoop(loopNumber++,
					                              PhTimeCode::timeFromString(elem.attribute("timecode"), _tcType)));
				// Reading cuts
				else if(elem.tagName() == "shot")
					_cuts.append(new PhStripCut(PhStripCut::Simple,
					                            PhTimeCode::timeFromString(elem.attribute("timecode"), _tcType)));
				else if(elem.tagName() == "line") {
					PhTime timeIn = -1;
					PhTime lastTime = -1;
					PhTime lastLinkedTime = -1;
					PhPeople *people = peopleMap[elem.attribute("role")];
					int track = elem.attribute("track").toInt();
					QString currentText = "";
					for(int j = 0; j < elem.childNodes().length(); j++) {
						if(elem.childNodes().at(j).isElement()) {
							QDomElement lineElem = elem.childNodes().at(j).toElement();
							if(lineElem.tagName() == "lipsync") {
								lastTime = PhTimeCode::timeFromString(lineElem.attribute("timecode"), _tcType);
								if(timeIn < 0)
									timeIn = lastTime;
								if(lineElem.attribute("link") != "off") {
									if(currentText.length()) {
										_texts.append(new PhStripText(lastLinkedTime, people, lastTime, track, currentText));
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
						PhTime time = lastLinkedTime + currentText.length();
						_texts.append(new PhStripText(lastLinkedTime, people, time, track, currentText));
						lastLinkedTime = time;
					}
					bool off = (elem.attribute("voice") == "off");
					_detects.append(new PhStripDetect(off, timeIn, people, lastTime, track));
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

PhTime PhStripDoc::readMosTime(QFile &f, PhTimeCodeType tcType)
{
	return PhFileTool::readInt(f, 4, "time") * PhTimeCode::timePerFrame(tcType) / 12;
}

PhStripText* PhStripDoc::readMosText(QFile &f, int level)
{
	int internLevel = 4;
	QString content = PhFileTool::readString(f, 2, "content");

	PhTime timeIn = _videoTimeIn + readMosTime(f, _tcType);;
	PhTime timeOut = _videoTimeIn + readMosTime(f, _tcType);;

	PhStripText* text = new PhStripText(timeIn, NULL, timeOut, 0, content);

	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");

	PHDBG(level) << PHNQ(PhTimeCode::stringFromTime(timeIn, _tcType))
	             << "->"
	             << PHNQ(PhTimeCode::stringFromTime(timeOut, _tcType))
	             << PHNQ(content);
	return text;
}

void PhStripDoc::readMosDetect(QFile &f, int level)
{
	int detectLevel = 5;
	PhTime timeIn = _videoTimeIn + readMosTime(f, _tcType);;
	PhTime timeOut = _videoTimeIn + readMosTime(f, _tcType);;
	for(int j = 0; j < 12; j++)
		PhFileTool::readShort(f, detectLevel);
	_detects.append(new PhStripDetect(false, timeIn, NULL, timeOut, 0));
	PHDBG(level) << PhTimeCode::stringFromTime(timeIn, _tcType)
	             << PhTimeCode::stringFromTime(timeOut, _tcType);
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

bool PhStripDoc::readMosTrack(QFile &f, QMap<int, PhPeople *> peopleMap, QMap<int, int> peopleTrackMap, int blocLevel, int textLevel, int detectLevel, int labelLevel, int level)
{
	QList<PhStripDetect*> detectLists;
	QList<PhStripText*> textList;
	int detectCount = PhFileTool::readInt(f, detectLevel, "track CDocBlocDetection count");

	if(detectCount) {
		if(!checkMosTag(f, blocLevel, MosDetect))
			return false;

		for(int i = 0; i < detectCount; i++) {
			if(i > 0)
				PhFileTool::readShort(f, level, "detect tag");
			readMosDetect(f, detectLevel);
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
			textList.append(readMosText(f, textLevel));
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
				textList.append(readMosText(f, textLevel));
			}
			break;
		case MosLabel:
			for(int i = 0; i < count; i++) {
				if(i > 0)
					PhFileTool::readShort(f, level, "label tag");
				PhTime labelTime = _videoTimeIn + readMosTime(f, _tcType);
				for(int j = 0; j < 6; j++)
					PhFileTool::readShort(f, level);
				PHDBG(labelLevel) << "label" << PhTimeCode::stringFromTime(labelTime, _tcType);
			}
			break;
		default:
			PHDEBUG << "!!!!!!!!!!!!!!! Unknown tag:" << PHNQ(QString::number(tag, 16)) << "!!!!!!!!!!!!!!!";
			f.close();
			return false;
		}
	}

	PHDBG(textLevel) << "Adding" << textList.count() << "texts";
	foreach(PhStripText* text, textList) {
		text->setPeople(peopleMap[peopleId]);
		text->setTrack(peopleTrackMap[peopleId]);
		_texts.append(text);
	}

	return true;
}

bool PhStripDoc::importMosFile(QString fileName)
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

	int level = 0;
	int ok = level;
	int propLevel = level;
	int peopleLevel = level;
	int textLevel = level;
	int detectLevel = level;
	int blocLevel = level;
	int cutLevel = level;
	int loopLevel = level;
	int labelLevel = level;

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

#warning /// @todo check strange number
	// read a number that makes a difference wether it's 3 or 4 later
	unsigned short strangeNumber1 = PhFileTool::readShort(f, blocLevel, "CDocOptionsProjet");


	if(!checkMosTag2(f, blocLevel, "CDocOptionsProjet"))
		return false;

	unsigned short type = PhFileTool::readInt(f, level, "type");
	bool drop = PhFileTool::readInt(f, level, "drop") != 0;
	switch(type) {
	case 0:
		if(drop)
			_tcType = PhTimeCodeType2398;
		else
			_tcType = PhTimeCodeType24;
		break;
	default:
		_tcType = PhTimeCodeType25;
		break;
	}
	PHDBG(ok) << "TC Type:" << _tcType;


	if(strangeNumber1 == 4) {
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

	this->setVideoFilePath(PhFileTool::readString(f, ok, "Video path"));
	this->setVideoTimeIn(readMosTime(f, _tcType));
	PHDBG(ok) << "Timestamp:" << PhTimeCode::stringFromTime(_videoTimeIn, _tcType);

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
			PhTime cutTime = _videoTimeIn + readMosTime(f, _tcType);
			PHDBG(cutLevel) << "cut:" << PhTimeCode::stringFromTime(cutTime, _tcType);
			_cuts.append(new PhStripCut(PhStripCut::Simple, cutTime));
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
		if(!readMosTrack(f, peopleMap, peopleTrackMap, blocLevel, textLevel, detectLevel, labelLevel, level))
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

			PhTime loopTime = _videoTimeIn + readMosTime(f, _tcType);;
			PhFileTool::readString(f, loopLevel, "loop name");
			_loops.append(new PhStripLoop(number, loopTime));
		}
	}

	for(int j = 0; j < 4; j++)
		PhFileTool::readShort(f, level, "after loop1");

	//	if(strangeNumber2 == 1) {
	//		for(int j = 0; j < 9; j++)
	//			PhFileTool::readShort(f, level, "after loop2");
	//	}

	if(!checkMosTag(f, blocLevel, MosBin))
		return false;

	for(int j = 0; j < 2; j++)
		PhFileTool::readShort(f, level);

	PHDEBUG << "_______________" << "reading ok" << "_______________";

	f.close();

	emit this->changed();

	return true;
}

bool PhStripDoc::openStripFile(QString fileName)
{
	PHDEBUG << fileName;
	bool result = false;

	QString extension = QFileInfo(fileName).suffix();
	// Try to open the document
	if(extension == "detx") {
		return importDetXFile(fileName);
	}
	else if(extension == "mos") {
		return importMosFile(fileName);
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
				_videoTimeIn = PhTimeCode::timeFromString(media.attribute("tcStamp"), _tcType);
				_forceRatio169 = media.attribute("forceRatio").toLower() == "yes";
			}
		}

		if(stripDocument.elementsByTagName("state").count()) {
			QDomElement state = stripDocument.elementsByTagName("state").at(0).toElement();
			_lastTime = PhTimeCode::timeFromString(state.attribute("lastTimeCode"), _tcType);
		}
	}
	return result;
}

bool PhStripDoc::saveStripFile(QString fileName, QString lastTC, bool forceRatio169)
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
				xmlWriter->writeAttribute("tcStamp", PhTimeCode::stringFromTime(_videoTimeIn, _tcType));
				if(forceRatio169)
					xmlWriter->writeAttribute("forceRatio", "yes");
				xmlWriter->writeCharacters(_videoPath);
				xmlWriter->writeEndElement();

				xmlWriter->writeStartElement("state");
				xmlWriter->writeAttribute("lastTimeCode", lastTC);
				xmlWriter->writeEndElement();
			}
			xmlWriter->writeEndElement();
		}
		xmlWriter->writeEndElement();

		xmlWriter->writeEndDocument();
		delete xmlWriter;
	}

	return true;
}

bool PhStripDoc::create(QString text, int nbPeople, int nbText, int nbTrack, PhTime videoTimeIn)
{
	this->reset();
	_title = "Generate file";
	_translatedTitle = "Fichier généré";
	_episode = "1";
	_season = "1";
	_tcType = PhTimeCodeType25;
	_videoTimeIn = videoTimeIn;
	_lastTime = _videoTimeIn;

	if (nbTrack > 4 || nbTrack < 1)
		nbTrack = 3;

	QStringList names;
	names.append("Actor");
	names.append("Actress");
	names.append("Jack");
	names.append("Jane");

	int nbNames = names.length();
	// Creation of the Peoples
	for (int i = 1; i <= nbPeople; i++) {
		PhPeople *people = new PhPeople(names.at(i % nbNames) + " " + QString::number(i), "black");
		_peoples.append(people);
	}

	int position = _videoTimeIn;
	// Creation of the text
	for (int i = 0; i < nbText; i++) {
		//Make people "talk" alternaly
		PhPeople *people = _peoples[i % nbPeople];

		int start = position;
		int end = start + text.length() * 1.20588 + 1;

		addText(people, start, end, text, i % nbTrack);

		// So the texts are all one after the other
		position += end - start;
	}

	emit changed();
	return true;
}

void PhStripDoc::reset()
{
	_peoples.clear();
	_cuts.clear();
	_detects.clear();
	_tcType = PhTimeCodeType25;
	_lastTime = 0;
	_loops.clear();
	_texts.clear();
	_title = "";
	_translatedTitle = "";
	_episode = "";
	_season = "";
	_videoPath = "";
	_videoTimeIn = 0;
	_authorName = "";
	_forceRatio169 = false;
	_generator = "";
	_mosNextTag = 0x8008;

	emit this->changed();
}

void PhStripDoc::addText(PhPeople * actor, PhTime timeIn, PhTime timeOut, QString sentence, int track)
{
	if(sentence != " " && sentence != "" ) {

		_texts.push_back(new PhStripText(timeIn, actor,
		                                 timeOut,
		                                 track, sentence));
	}
}
bool PhStripDoc::forceRatio169() const
{
	return _forceRatio169;
}

PhPeople *PhStripDoc::peopleByName(QString name)
{
	foreach(PhPeople* people, _peoples)
	{
		if(people && people->name() == name)
			return people;
	}
	return NULL;
}

PhStripText *PhStripDoc::nextText(PhTime time)
{
	PhStripText * result = NULL;
	foreach(PhStripText* text, _texts)
	{
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
	foreach(PhStripText* text, _texts)
	{
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
	foreach(PhStripText* text, _texts)
	{
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

	foreach(PhStripText* text, _texts)
	{
		if((text->timeIn() < time) && (text->timeIn() > previousTextTime) )
			previousTextTime = text->timeIn();
	}

	return previousTextTime;
}

PhTime PhStripDoc::previousLoopTime(PhTime time)
{
	PhTime previousLoopTime = PHTIMEMIN;

	foreach(PhStripLoop* loop, _loops)
	{
		if((loop->timeIn() < time) && (loop->timeIn() > previousLoopTime) )
			previousLoopTime = loop->timeIn();
	}

	return previousLoopTime;
}

PhTime PhStripDoc::previousCutTime(PhTime time)
{
	PhTime previousCutTime = PHTIMEMIN;

	foreach(PhStripCut* cut, _cuts)
	{
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

	foreach(PhStripText* text, _texts)
	{
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

	foreach(PhStripLoop* loop, _loops)
	{
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

	foreach(PhStripCut* cut, _cuts)
	{
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
	foreach(PhStripLoop* loop, _loops)
	{
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

PhTimeCodeType PhStripDoc::timeCodeType()
{
	return _tcType;
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

PhTime PhStripDoc::videoTimeIn()
{
	return _videoTimeIn;
}

PhTime PhStripDoc::lastTime()
{
	return _lastTime;
}

QList<PhStripText *> PhStripDoc::texts()
{
	return _texts;
}

QList<PhStripText *> PhStripDoc::texts(PhPeople *people)
{
	QList<PhStripText*> result;
	foreach(PhStripText *text, _texts) {
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

void PhStripDoc::setVideoTimeIn(PhTime time)
{
	_videoTimeIn = time;
}

void PhStripDoc::setVideoFilePath(QString videoFilePath)
{
	_videoPath = videoFilePath;
}

QList<PhStripCut *> PhStripDoc::cuts()
{
	return _cuts;
}

