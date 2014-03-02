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
	_timeScale = 25.00;

	// Reading the header
	if(detX.elementsByTagName("header").count()) {
		QDomElement header = detX.elementsByTagName("header").at(0).toElement();

		// Read the Cappella version
		if(header.elementsByTagName("cappella").count())
			_generator += " v" + detX.elementsByTagName("cappella").at(0).toElement().attribute("version");

		// Reading the title
		if(header.elementsByTagName("title").count())
			_title = header.elementsByTagName("title").at(0).toElement().text();

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

bool PhStripDoc::checkMosTag(QFile &f, int logLevel, QString expected)
{
	QString name = PhFileTool::readString(f, logLevel, expected);
	if(name != expected) {
		PHDEBUG << "!!!!!!!!!!!!!!!" << "Error reading " << name<< " instead of" << expected << "!!!!!!!!!!!!!!!";
		f.close();
		return false;
	}
	return true;
}

bool PhStripDoc::checkMosWord(QFile &f, int logLevel, unsigned short expected)
{
	QString name = QString::number(expected, 16);
	unsigned short word = PhFileTool::readShort(f, logLevel, PHNQ(name));
	if(word != expected) {
		PHDEBUG << "!!!!!!!!!!!!!!!" << "Error reading " << PHNQ(QString::number(word, 16)) << "instead of" << name << "!!!!!!!!!!!!!!!";
		f.close();
		return false;
	}
	return true;
}

void PhStripDoc::readMosText(QFile &f, int logLevel)
{
	int level = 4;
	PhFileTool::readShort(f, level, "text start");
	QString content = PhFileTool::readString(f, 2, "content");

	PhFrame frameIn = _videoFrameStamp + PhFileTool::readInt(f, 2, "tcin") / 12;
	PhFrame frameOut = _videoFrameStamp + PhFileTool::readInt(f, 2, "tcout") / 12;

#warning TODO decode people and track number
	PhStripText* text = new PhStripText(frameIn, NULL, frameOut, 0, content);

	PhFileTool::readInt(f, level, "text");
	PhFileTool::readInt(f, level, "text");
	PhFileTool::readInt(f, level, "text");
	PhFileTool::readInt(f, level, "text");
	PhFileTool::readInt(f, level, "text");
	PhFileTool::readInt(f, level, "text");

	PHDBG(logLevel) << PHNQ(PhTimeCode::stringFromFrame(frameIn, _tcType))
	                << "->"
	                << PHNQ(PhTimeCode::stringFromFrame(frameOut, _tcType))
	                << PHNQ(content);
	_texts.append(text);
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

	int logLevel = 2;
	int ok = 0;
	int propLogLevel = logLevel;
	int peopleLogLevel = logLevel;
	int textLogLevel = logLevel;
	int blocLogLevel = 0;
	int loopLogLevel = logLevel;

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	if(!checkMosTag(f, blocLogLevel, "NOBLURMOSAIC"))
		return false;

	if(!checkMosWord(f, logLevel, 0xffff))
		return false;

	int strangeNumber3 = PhFileTool::readShort(f, ok, "strangeNumber3");

	if(!checkMosTag(f, blocLogLevel, "CMosaicDoc"))
		return false;

	if(!checkMosWord(f, logLevel, 0xffff) || !checkMosWord(f, logLevel, 0x1))
		return false;

	if(!checkMosTag(f, blocLogLevel, "CDocProjet"))
		return false;

	if(!checkMosWord(f, logLevel, 0xffff) || !checkMosWord(f, logLevel, 0x1))
		return false;

	if(!checkMosTag(f, blocLogLevel, "CDocProprietes"))
		return false;

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, propLogLevel, "Titre de la versio originale");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	_title = PhFileTool::readString(f, propLogLevel, "Titre de la version adaptée");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	_season = PhFileTool::readString(f, propLogLevel, "Saison");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	_episode = PhFileTool::readString(f, propLogLevel, "Episode/bobine");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, logLevel, "Titre vo episode");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, propLogLevel, "Titre adapté de l'épisode");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, propLogLevel, "Durée");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, propLogLevel, "Date");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, propLogLevel, "Client");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, propLogLevel, "Commentaires");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, propLogLevel, "Détecteur");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	_authorName = PhFileTool::readString(f, propLogLevel, "Auteur");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, logLevel, "Studio");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, logLevel, "D.A.");

	if(!checkMosWord(f, logLevel, 0xfeff))
		return false;

	PhFileTool::readString(f, logLevel, "Ingénieur du son");

	if(!checkMosWord(f, logLevel, 0xffff))
		return false;

#warning /// @todo check strange number
	// read a number that makes a difference wether it's 3 or 4 later
	unsigned short strangeNumber1 = PhFileTool::readShort(f, ok, "strangeNumber");

	if(!checkMosTag(f, blocLogLevel, "CDocOptionsProjet"))
		return false;

	unsigned short type = PhFileTool::readInt(f, logLevel, "type");
	bool drop = PhFileTool::readInt(f, logLevel, "drop") != 0;
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
	PHDBG(ok) << "tc type:" << _tcType;

	if(strangeNumber1 == 4) {
//		qDebug() << "reading extrasection ???";
//		PhFileTool::readInt(f, logLevel, "loop continuous numbering");
		PhFileTool::readShort(f, logLevel);
		PhFileTool::readShort(f, logLevel);
	}

	for(int j = 0; j < 12; j++)
		PhFileTool::readShort(f, logLevel);

	if(!checkMosTag(f, blocLogLevel, "CDocFilm"))
		return false;

	unsigned short peopleNumber = PhFileTool::readShort(f, ok, "people count");

	for(int j = 0; j < 3; j++)
		PhFileTool::readShort(f, logLevel);

	if(!checkMosTag(f, blocLogLevel, "CDocPersonnage"))
		return false;

	for(int i = 0; i < peopleNumber; i++) {
		PhFileTool::readInt(f, peopleLogLevel, "index");

		PhFileTool::readShort(f, logLevel);

		QString name = PhFileTool::readString(f, peopleLogLevel, "name");
		_peoples[name] = new PhPeople(name, "#000000");

		PhFileTool::readInt(f, ok, "line number");
		for(int j = 0; j < 6; j++)
			PhFileTool::readShort(f, logLevel);

		if(PhFileTool::readShort(f, logLevel, "test") != 0x8006) {
			PhFileTool::readString(f, peopleLogLevel, "date 1");
			PhFileTool::readShort(f, logLevel);
		}
	}

	PhFileTool::readShort(f, logLevel);
	if(strangeNumber1 == 4) {
		qDebug() << "reading extrasection ???";

		PhFileTool::readShort(f, logLevel);
		PhFileTool::readShort(f, logLevel);
	}

	if(!checkMosTag(f, blocLogLevel, "CDocVideo"))
		return false;

	PhFileTool::readShort(f, logLevel);

	this->setVideoPath(PhFileTool::readString(f, ok, "Video path"));
	this->setVideoTimestamp(PhFileTool::readInt(f, logLevel, "timestamp") / 12);
	PHDBG(ok) << "Timestamp:" << PhTimeCode::stringFromFrame(_videoFrameStamp, _tcType);

	for(int j = 0; j < 2; j++)
		PhFileTool::readShort(f, logLevel);

	int cutLogLevel = logLevel;
	if(strangeNumber1 == 4) {
		PHDBG(ok) << "reading extrasection ???";

		PhFileTool::readShort(f, logLevel);
		PhFileTool::readShort(f, logLevel);
		unsigned short cutCount = PhFileTool::readInt(f, ok, "cut count");
		PhFileTool::readShort(f, logLevel);
		PhFileTool::readShort(f, logLevel);
		if(!checkMosTag(f, blocLogLevel, "CDocPlan"))
			return false;

		for(int j = 0; j < cutCount; j++) {
			PhFrame cutFrame = _videoFrameStamp + PhFileTool::readInt(f, logLevel, "cut frame") / 12;
			PHDBG(cutLogLevel) << "cut:" << PhTimeCode::stringFromFrame(cutFrame, _tcType);
			PhFileTool::readShort(f, logLevel, "cut type?");
			_cuts.append(new PhStripCut(PhStripCut::Simple, cutFrame));
		}
		PhFileTool::readShort(f, logLevel, "end cut");
	}
	else {
		for(int j = 0; j < 4; j++)
			PhFileTool::readShort(f, logLevel);
	}


	for(int j = 0; j < 4; j++)
		PhFileTool::readShort(f, logLevel);

	if(!checkMosTag(f, blocLogLevel, "CDocDoublage"))
		return false;

	for(int j = 0; j < 12; j++)
		PhFileTool::readShort(f, logLevel);

	if(!checkMosTag(f, blocLogLevel, "CDocPiste"))
		return false;

	if(strangeNumber1 == 3) {
		for(int j = 0; j < 4; j++)
			PhFileTool::readShort(f, logLevel);

		if(!checkMosTag(f, blocLogLevel, "CDocBlocDetection"))
			return false;

		for(int j = 0; j < 65; j++)
			PhFileTool::readShort(f, logLevel);
	}

	for(int j = 0; j < 6; j++)
		PhFileTool::readShort(f, logLevel);

	if(!checkMosTag(f, blocLogLevel, "CDocLangue"))
		return false;

	PhFileTool::readShort(f, logLevel);
	PhFileTool::readShort(f, logLevel);
#warning /// @todo check why 1
	int strangeNumber2 = PhFileTool::readShort(f, ok, "strange number 2");
	if(strangeNumber2 == 1) {
		for(int j = 0; j < 6; j++)
			PhFileTool::readShort(f, logLevel);
	}
	PhFileTool::readShort(f, logLevel);

	if(!checkMosTag(f, blocLogLevel, "CDocBlocTexte"))
		return false;

	while(true) {
		readMosText(f, textLogLevel);
		if(PhFileTool::readShort(f, logLevel) != 0x800c)
			break;
	}

	for(int j = 0; j < 4; j++)
		PhFileTool::readShort(f, logLevel);

	if(strangeNumber2 == 1) {
		if(!checkMosTag(f, blocLogLevel, "CDocBlocDetection"))
			return false;
		for(int i = 0; i < 24; i++) {
			PhFrame frameIn = _videoFrameStamp + PhFileTool::readInt(f, logLevel, "tcin") / 12;
			PhFrame frameOut = _videoFrameStamp + PhFileTool::readInt(f, logLevel, "tcout") / 12;
			for(int j = 0; j < 13; j++)
				PhFileTool::readShort(f, logLevel);
			PHDBG(logLevel) << PhTimeCode::stringFromFrame(frameIn, _tcType)
			                << PhTimeCode::stringFromFrame(frameOut, _tcType);
		}
		for(int j = 0; j < 10; j++)
			PhFileTool::readShort(f, logLevel);
	}
	else
		PhFileTool::readShort(f, logLevel);

	if(!checkMosTag(f, blocLogLevel, "CDocEtiquetteNom"))
		return false;

	PhFileTool::readInt(f, ok, "tcin");
	for(int j = 0; j < 7; j++)
		PhFileTool::readShort(f, logLevel);

	if(strangeNumber2 == 1) {
		for(int j = 0; j < 20; j++)
			PhFileTool::readShort(f, logLevel, "before text");
		while(true) {
			readMosText(f, textLogLevel);
			if(PhFileTool::readShort(f, logLevel) != 0x800c)
				break;
		}
		PhFileTool::readShort(f, logLevel, "after text");
		PhFileTool::readShort(f, logLevel, "after text");
	}
	else if(strangeNumber1 == 4) {
		qDebug() << "reading extrasection containing phrase 2:";
		for(int j = 0; j < 10; j++)
			PhFileTool::readShort(f, logLevel);

#warning TODO insert text only one time
		readMosText(f, textLogLevel);
		PhFileTool::readShort(f, logLevel);

		for(int j = 0; j < 17; j++)
			PhFileTool::readShort(f, logLevel);
	}

	int loopCount = PhFileTool::readShort(f, ok, "loop count");
	for(int i = 0; i < loopCount; i++) {
		PhFileTool::readString(f, loopLogLevel, "CDocBoucle");
		int number = PhFileTool::readInt(f, loopLogLevel, "loop number");
		PhFrame loopFrame = _videoFrameStamp + PhFileTool::readInt(f, loopLogLevel, "loop tc") / 12;
		_loops.append(new PhStripLoop(loopCount, loopFrame));
	}

	for(int j = 0; j < 10; j++)
		PhFileTool::readShort(f, logLevel, "after loop1");

	if(strangeNumber2 == 1) {
		for(int j = 0; j < 9; j++)
			PhFileTool::readShort(f, logLevel, "after loop2");
	}

	if(!checkMosTag(f, blocLogLevel, "CDocChutier"))
		return false;

	for(int j = 0; j < 2; j++)
		PhFileTool::readShort(f, logLevel);

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
	_timeScale = 25.00;
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
	_timeScale = 25; //TODO fix me
	_title = "";
	_translatedTitle = "";
	_episode = "";
	_season = "";
	_videoPath = "";
	_videoFrameStamp = 0;
	_authorName = "";
	_forceRatio169 = false;

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
		if(people->getName() == name)
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

