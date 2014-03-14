/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include <QFile>
#include <QFileInfo>
#include "PhTools/PhFileTool.h"
#include "PhStripDoc.h"

PhStripDoc::PhStripDoc(QObject *parent) :
	QObject(parent)
{
	reset();
}


bool PhStripDoc::importDetX(QString fileName)
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
			_title = QFileInfo(fileName).fileName().split(".").first();


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
			// Reading the video framestamp
			_videoFrameStamp = PhTimeCode::frameFromString(videoFile.attribute("timestamp"), _tcType);
		}

		// Reading the last position
		if(header.elementsByTagName("last_position").count()) {
			QDomElement lastPosition = header.elementsByTagName("last_position").at(0).toElement();
			_lastFrame = PhTimeCode::frameFromString(lastPosition.attribute("timecode"), _tcType);
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

	// Reading the "role" lists
	if(detX.elementsByTagName("roles").count()) {
		QDomElement roles = detX.elementsByTagName("roles").at(0).toElement();
		QDomNodeList roleList = roles.elementsByTagName("role");
		for (int i = 0; i < roleList.length(); i++) {
			QDomElement role = roleList.at(i).toElement();
			PhPeople *people = new PhPeople(role.attribute("name"), role.attribute("color"));

			//Currently using id as key instead of name
			_peoples[role.attribute("id")] = people;
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
					                              PhTimeCode::frameFromString(elem.attribute("timecode"), _tcType)));
				// Reading cuts
				else if(elem.tagName() == "shot")
					_cuts.append(new PhStripCut(PhStripCut::Simple,
					                            PhTimeCode::frameFromString(elem.attribute("timecode"), _tcType)));
				else if(elem.tagName() == "line") {
					PhFrame frameIn = -1;
					PhFrame lastFrame = -1;
					PhFrame lastLinkedFrame = -1;
					PhPeople *people = _peoples[elem.attribute("role")];
					int track = elem.attribute("track").toInt();
					QString currentText = "";
					for(int j = 0; j < elem.childNodes().length(); j++) {
						if(elem.childNodes().at(j).isElement()) {
							QDomElement lineElem = elem.childNodes().at(j).toElement();
							if(lineElem.tagName() == "lipsync") {
								lastFrame = PhTimeCode::frameFromString(lineElem.attribute("timecode"), _tcType);
								if(frameIn < 0)
									frameIn = lastFrame;
								if(lineElem.attribute("link") != "off") {
									if(currentText.length()) {
										_texts.append(new PhStripText(lastLinkedFrame, people, lastFrame, track, currentText));
										currentText = "";
									}
									lastLinkedFrame = lastFrame;
								}
							}
							else if(lineElem.tagName() == "text")
								currentText += lineElem.text();
						}
					}
					// Handling line with no lipsync out
					if(currentText.length()) {
						PhFrame frame = lastLinkedFrame + currentText.length();
						_texts.append(new PhStripText(lastLinkedFrame, people, frame, track, currentText));
						lastLinkedFrame = frame;
					}
					bool off = (elem.attribute("voice") == "off");
					_detects.append(new PhStripDetect(off, frameIn, people, lastFrame, track));
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

bool PhStripDoc::checkMosWord(QFile &f, int level, unsigned short expected)
{
	QString name = QString::number(expected, 16);
	unsigned short word = PhFileTool::readShort(f, level, PHNQ(name));
	if(word != expected) {
		PHDEBUG << "!!!!!!!!!!!!!!!" << "Error reading " << PHNQ(QString::number(word, 16)) << "instead of" << name << "!!!!!!!!!!!!!!!";
		f.close();
		return false;
	}
	return true;
}

PhStripText* PhStripDoc::readMosText(QFile &f, int level)
{
	int internLevel = 4;
	QString content = PhFileTool::readString(f, 2, "content");

	PhFrame frameIn = _videoFrameStamp + PhFileTool::readInt(f, 2, "tcin") / 12;
	PhFrame frameOut = _videoFrameStamp + PhFileTool::readInt(f, 2, "tcout") / 12;

#warning TODO decode people and track number
	PhStripText* text = new PhStripText(frameIn, NULL, frameOut, 0, content);

	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");
	PhFileTool::readInt(f, internLevel, "text");

	PHDBG(level) << PHNQ(PhTimeCode::stringFromFrame(frameIn, _tcType))
	             << "->"
	             << PHNQ(PhTimeCode::stringFromFrame(frameOut, _tcType))
	             << PHNQ(content);
	return text;
}

void PhStripDoc::readMosDetect(QFile &f, int level)
{
	int detectLevel = 5;
	PhFrame frameIn = _videoFrameStamp + PhFileTool::readInt(f, detectLevel, "tcin") / 12;
	PhFrame frameOut = _videoFrameStamp + PhFileTool::readInt(f, detectLevel, "tcout") / 12;
	for(int j = 0; j < 12; j++)
		PhFileTool::readShort(f, detectLevel);
	_detects.append(new PhStripDetect(false, frameIn, NULL, frameOut, 0));
	PHDBG(level) << PhTimeCode::stringFromFrame(frameIn, _tcType)
	             << PhTimeCode::stringFromFrame(frameOut, _tcType);
}

bool PhStripDoc::readMosProperties(QFile &f, int level)
{
	PhFileTool::readString(f, level, "Titre de la versio originale");

	_title = PhFileTool::readString(f, level, "Titre de la version adaptée");
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
				PhFrame labelFrame = _videoFrameStamp + PhFileTool::readInt(f, level, "label tcin") / 12;
				for(int j = 0; j < 6; j++)
					PhFileTool::readShort(f, level);
				PHDBG(labelLevel) << "label" << PhTimeCode::stringFromFrame(labelFrame, _tcType);
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

bool PhStripDoc::importMos(QString fileName)
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

	PhFileTool::readShort(f, blocLevel, "CMosaicDoc");
	int strangeNumber3 = PhFileTool::readShort(f, blocLevel, "CMosaicDoc");

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
		peopleMap[peopleId] = _peoples[name] = new PhPeople(name, "#000000");

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

	this->setVideoPath(PhFileTool::readString(f, ok, "Video path"));
	this->setVideoTimestamp(PhFileTool::readInt(f, level, "timestamp") / 12);
	PHDBG(ok) << "Timestamp:" << PhTimeCode::stringFromFrame(_videoFrameStamp, _tcType);

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
			PhFrame cutFrame = _videoFrameStamp + PhFileTool::readInt(f, level, "cut frame") / 12;
			PHDBG(cutLevel) << "cut:" << PhTimeCode::stringFromFrame(cutFrame, _tcType);
			_cuts.append(new PhStripCut(PhStripCut::Simple, cutFrame));
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

			PhFrame loopFrame = _videoFrameStamp + PhFileTool::readInt(f, loopLevel, "loop tc") / 12;
			PhFileTool::readString(f, loopLevel, "loop name");
			_loops.append(new PhStripLoop(number, loopFrame));
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

	return true;
}

bool PhStripDoc::openStripFile(QString fileName)
{
	bool succeed = false;

	QString extension = QFileInfo(fileName).suffix();
	// Try to open the document
	if(extension == "detx") {
		return importDetX(fileName);
	}
	else if(extension == "mos") {
		return importMos(fileName);
	}
	else if(extension == "strip") {
		QFile xmlFile(fileName);
		if(!xmlFile.open(QIODevice::ReadOnly)) {
			PHDEBUG << "Unable to open" << fileName;
			return false;
		}

		// Loading the DOM
		QDomDocument *domDoc = new QDomDocument();
		if (!domDoc->setContent(&xmlFile)) {
			xmlFile.close();
			PHDEBUG << "The XML document seems to be bad formed" << fileName;
			return false;
		}

		PHDEBUG << ("Start parsing " + fileName);
		QDomElement stripDocument = domDoc->documentElement();

		if(stripDocument.tagName() != "strip") {
			xmlFile.close();
			PHDEBUG << "Bad root element :" << stripDocument.tagName();
			return false;
		}

		QDomElement metaInfo =  stripDocument.elementsByTagName("meta").at(0).toElement();
		// Reading the header
		if(stripDocument.elementsByTagName("meta").count()) {
			for(int i = 0; i < stripDocument.elementsByTagName("media").count(); i++) {
				QDomElement line = metaInfo.elementsByTagName("media").at(i).toElement();
				PHDEBUG << "line" << line.attribute("type");
				if(line.attribute("type") == "detx")
					succeed = importDetX(line.text());

				if(line.attribute("type")  == "video") {
					_videoPath = line.text();
					_videoFrameStamp = PhTimeCode::frameFromString(line.attribute("tcStamp"), _tcType);
					_forceRatio169 = line.attribute("forceRatio") == "YES";
				}
			}
		}
		_lastFrame = PhTimeCode::frameFromString(metaInfo.elementsByTagName("state").at(0).toElement().attribute("lastTimeCode"), _tcType);
	}
	return succeed;

}

bool PhStripDoc::saveStrip(QString fileName, QString lastTC, bool forceRatio169)
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
		xmlWriter->writeStartElement("strip");
		{
			xmlWriter->writeStartElement("meta");
			{
				xmlWriter->writeStartElement("generator");
				xmlWriter->writeAttribute("name", "Joker");
#ifdef APP_VERSION
				xmlWriter->writeAttribute("version", APP_VERSION);
#endif
				xmlWriter->writeEndElement();

				xmlWriter->writeStartElement("media");
				xmlWriter->writeAttribute("type", "detx");
				xmlWriter->writeCharacters(getFilePath());
				xmlWriter->writeEndElement();

				xmlWriter->writeStartElement("media");
				xmlWriter->writeAttribute("type", "video");
				xmlWriter->writeAttribute("tcStamp", PhTimeCode::stringFromFrame(_videoFrameStamp, PhTimeCodeType25));
				if(forceRatio169)
					xmlWriter->writeAttribute("forceRatio", "YES");
				else
					xmlWriter->writeAttribute("forceRatio", "NO");
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

bool PhStripDoc::createDoc(QString text, int nbPeople, int nbText, int nbTrack, PhTime videoTimeCode)
{
	this->reset();
	_title = "Generate file";
	_translatedTitle = "Fichier généré";
	_episode = "1";
	_season = "1";
	_tcType = PhTimeCodeType25;
	_videoFrameStamp = videoTimeCode;
	_lastFrame = _videoFrameStamp;

	if (nbTrack > 4 || nbTrack < 1)
		nbTrack = 3;

	QStringList names;
	names.append("Actor");
	names.append("Actress");
	names.append("Jack");
	names.append("Jane");

	int nbNames = names.length();
	QStringList idList;
	// Creation of the Peoples
	for (int i = 1; i <= nbPeople; i++) {
		PhPeople *people = new PhPeople(names.at(i % nbNames) + " " + QString::number(i), "black");
		_peoples[people->getName()] = people;
		idList.append(people->getName());
	}

	int position = _videoFrameStamp;
	// Creation of the text
	for (int i = 0; i < nbText; i++) {
		//Make people "talk" alternaly
		QString id = _peoples[idList.at(i % nbPeople)]->getName();

		int start = position;
		int end = start + text.length() * 1.20588 + 1;

		addText(_peoples[id], start, end,
		        text, i % nbTrack);

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
	_lastFrame = 0;
	_loops.clear();
	_nbTexts = 0;
	_texts.clear();
	_title = "";
	_translatedTitle = "";
	_episode = "";
	_season = "";
	_videoPath = "";
	_videoFrameStamp = 0;
	_authorName = "";
	_forceRatio169 = false;
	_mosNextTag = 0x8008;

	emit this->changed();
}

void PhStripDoc::addText(PhPeople * actor, PhTime start, PhTime end, QString sentence, int track)
{

	if(sentence != " " && sentence != "" ) {

		_texts.push_back(new PhStripText(start, actor,
		                                 end,
		                                 track, sentence));
		_nbTexts++;
	}
}
bool PhStripDoc::forceRatio169() const
{
	return _forceRatio169;
}

int PhStripDoc::getNbTexts()
{
	return _nbTexts;
}

PhPeople *PhStripDoc::getPeopleByName(QString name)
{
	foreach(PhPeople* people, _peoples)
	{
		if(people && people->getName() == name)
			return people;
	}
	return NULL;
}

PhStripText *PhStripDoc::getNextText(PhFrame frame)
{
	PhStripText * result = NULL;
	foreach(PhStripText* text, _texts)
	{
		if(text->getTimeIn() > frame) {
			if(!result || (text->getTimeIn() < result->getTimeIn()) )
				result = text;
		}
	}
	return result;
}

PhStripText *PhStripDoc::getNextText(PhFrame frame, PhPeople *people)
{
	PhStripText * result = NULL;
	foreach(PhStripText* text, _texts)
	{
		if((text->getPeople() == people) && (text->getTimeIn() > frame)) {
			if(!result || (text->getTimeIn() < result->getTimeIn()) )
				result = text;
		}
	}
	return result;
}

PhStripText *PhStripDoc::getNextText(PhFrame frame, QList<PhPeople *> peopleList)
{
	PhStripText * result = NULL;
	foreach(PhStripText* text, _texts)
	{
		if(peopleList.contains(text->getPeople()) && (text->getTimeIn() > frame)) {
			if(!result || (text->getTimeIn() < result->getTimeIn()) )
				result = text;
		}
	}
	return result;
}

PhFrame PhStripDoc::getPreviousTextFrame(PhFrame frame)
{
	PhFrame previousTextFrame = PHFRAMEMIN;

	foreach(PhStripText* text, _texts)
	{
		if((text->getTimeIn() < frame) && (text->getTimeIn() > previousTextFrame) )
			previousTextFrame = text->getTimeIn();
	}

	return previousTextFrame;
}

PhFrame PhStripDoc::getPreviousLoopFrame(PhFrame frame)
{
	PhFrame previousLoopFrame = PHFRAMEMIN;

	foreach(PhStripLoop* loop, _loops)
	{
		if((loop->getTimeIn() < frame) && (loop->getTimeIn() > previousLoopFrame) )
			previousLoopFrame = loop->getTimeIn();
	}

	return previousLoopFrame;
}

PhFrame PhStripDoc::getPreviousCutFrame(PhFrame frame)
{
	PhFrame previousCutFrame = PHFRAMEMIN;

	foreach(PhStripCut* cut, _cuts)
	{
		if((cut->getTimeIn() < frame) && (cut->getTimeIn() > previousCutFrame) )
			previousCutFrame = cut->getTimeIn();
	}

	return previousCutFrame;
}

PhFrame PhStripDoc::getPreviousElementFrame(PhFrame frame)
{
	PhFrame previousElementFrame = getPreviousCutFrame(frame);

	if(getPreviousLoopFrame(frame) > previousElementFrame)
		previousElementFrame = getPreviousLoopFrame(frame);

	if(getPreviousTextFrame(frame) > previousElementFrame)
		previousElementFrame = getPreviousTextFrame(frame);

	return previousElementFrame;
}

PhFrame PhStripDoc::getNextTextFrame(PhFrame frame)
{
	PhFrame nextTextFrame = PHFRAMEMAX;

	foreach(PhStripText* text, _texts)
	{
		if((text->getTimeIn() > frame) && (text->getTimeIn() < nextTextFrame) )
			nextTextFrame = text->getTimeIn();
		else if(text->getTimeIn() > nextTextFrame)
			return nextTextFrame;
	}

	return nextTextFrame;
}

PhFrame PhStripDoc::getNextLoopFrame(PhFrame frame)
{
	PhFrame nextLoopFrame = PHFRAMEMAX;

	foreach(PhStripLoop* loop, _loops)
	{
		if((loop->getTimeIn() > frame) && (loop->getTimeIn() < nextLoopFrame) )
			nextLoopFrame = loop->getTimeIn();
		else if(loop->getTimeIn() > nextLoopFrame)
			return nextLoopFrame;
	}

	return nextLoopFrame;
}

PhFrame PhStripDoc::getNextCutFrame(PhFrame frame)
{
	PhFrame nextCutFrame = PHFRAMEMAX;

	foreach(PhStripCut* cut, _cuts)
	{
		if((cut->getTimeIn() > frame) && (cut->getTimeIn() < nextCutFrame) )
			nextCutFrame = cut->getTimeIn();
		else if(cut->getTimeIn() > nextCutFrame)
			return nextCutFrame;
	}

	return nextCutFrame;
}

PhFrame PhStripDoc::getNextElementFrame(PhFrame frame)
{
	PhFrame nextElementFrame = getNextCutFrame(frame);

	if(getNextLoopFrame(frame) < nextElementFrame)
		nextElementFrame = getNextLoopFrame(frame);

	if(getNextTextFrame(frame) < nextElementFrame)
		nextElementFrame = getNextTextFrame(frame);

	return nextElementFrame;
}

PhFrame PhStripDoc::getFrameIn()
{
	return getNextElementFrame(0);
}

PhFrame PhStripDoc::getFrameOut()
{
	return getPreviousElementFrame(PHFRAMEMAX);
}

PhStripLoop *PhStripDoc::getNextLoop(PhFrame frame)
{
	foreach(PhStripLoop* loop, _loops)
	{
		if(loop->getTimeIn() > frame)
			return loop;
	}
	return NULL;
}

PhStripLoop *PhStripDoc::getPreviousLoop(PhFrame frame)
{
	int i = _loops.count() - 1;
	while(i >= 0) {
		if(_loops.at(i)->getTimeIn() < frame)
			return _loops.at(i);
		i--;
	}
	return NULL;
}

QString PhStripDoc::getFilePath()
{
	return _filePath;
}

QString PhStripDoc::getVideoPath()
{
	return _videoPath;
}

QList<QString> PhStripDoc::getMetaKey()
{
	return _metaInformation.keys();
}

QString PhStripDoc::getMetaInformation(QString key)
{
	return _metaInformation[key];
}

PhTimeCodeType PhStripDoc::getTCType()
{
#warning /// @todo rename to timeCodeType()
	return _tcType;
}

QString PhStripDoc::getTitle()
{
	return _title;
}

QString PhStripDoc::getTranslatedTitle()
{
	return _translatedTitle;
}

QString PhStripDoc::getEpisode()
{
	return _episode;
}

QString PhStripDoc::getSeason()
{
	return _season;
}

PhTime PhStripDoc::getVideoTimestamp()
{
	return _videoFrameStamp;
}

PhFrame PhStripDoc::getLastFrame()
{
	return _lastFrame;
}

QMap<QString, PhPeople *> PhStripDoc::getPeoples()
{
	return _peoples;
}

QList<PhStripText *> PhStripDoc::getTexts()
{
	return _texts;
}

QList<PhStripText *> PhStripDoc::getTexts(PhPeople *people)
{
	QList<PhStripText*> result;
	foreach(PhStripText *text, _texts) {
		if(text->getPeople() == people)
			result.append(text);
	}
	return result;
}

QList<PhStripLoop *> PhStripDoc::getLoops()
{
	return _loops;
}

QList<PhStripDetect *> PhStripDoc::getDetects(PhFrame frameIn, PhFrame frameOut)
{
	QList<PhStripDetect*> result;
	foreach(PhStripDetect *detect, this->_detects) {
		if((detect->getTimeIn() >= frameIn) && (detect->getTimeOut() < frameOut))
			result.append(detect);
	}

	return result;
}

QList<PhStripDetect *> PhStripDoc::getPeopleDetects(PhPeople *people, PhFrame frameIn, PhFrame frameOut)
{
	QList<PhStripDetect *> result;
	foreach(PhStripDetect *detect, this->getDetects(frameIn, frameOut)) {
		if(detect->getPeople() == people)
			result.append(detect);
	}
	return result;
}

void PhStripDoc::setTitle(QString title)
{
	_title = title;
}

void PhStripDoc::setVideoTimestamp(PhFrame videoFramestamp)
{
	_videoFrameStamp = videoFramestamp;
}

void PhStripDoc::setVideoPath(QString videoPath)
{
	_videoPath = videoPath;
}

QList<PhStripCut *> PhStripDoc::getCuts()
{
	return _cuts;
}

