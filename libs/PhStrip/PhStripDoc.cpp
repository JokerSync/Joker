/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include "PhStripDoc.h"


PhStripDoc::PhStripDoc(QString filename)
{
    openDetX(filename);
}


bool PhStripDoc::openDetX(QString filename)
{

    QDomDocument *DetX = new QDomDocument("/text.xml"); // Création de l'objet DOM
    QFile xml_doc(filename);// On choisit le fichier contenant les informations XML.
    if(!xml_doc.open(QIODevice::ReadOnly))// Si l'on n'arrive pas à ouvrir le fichier XML.
    {
        qDebug() << ("Le document XML n'a pas pu être ouvert. Vérifiez que le nom est le bon et que le document est bien placé");
        return false;
    }
    if (!DetX->setContent(&xml_doc)) // Si l'on n'arrive pas à associer le fichier XML à l'objet DOM.
    {
        xml_doc.close();
        qDebug() << ("Le document XML n'a pas pu être attribué à l'objet QDomDocument.");
        return false;
    }
    else{
        qDebug() << ("The file \"" + filename + "\" is now open.");
        qDebug("-----------------------------");
    }

    //Find the first title
    _title = DetX->elementsByTagName("title").at(0).toElement().text();
    //Find possible subtitles (start from title'num') with  2 <= num <= +∞
    int i = 2;
    while(DetX->elementsByTagName("title" + QString::number(i)).at(0).toElement().text() != ""){
        _title += " - " + DetX->elementsByTagName("title" + QString::number(i)).at(0).toElement().text();
        i++;
    }

    //Find the videoPath
    _videoPath = DetX->elementsByTagName("videofile").at(0).toElement().text();

    //Find the videoTimeStamp
    _videoTimestamp = PhTimeCode::frameFromString(DetX->elementsByTagName("videofile").at(0).toElement().attribute("timestamp"),
                                                  PhTimeCodeType25);

    //Find the last position
    _lastPosition = PhTimeCode::frameFromString(DetX->elementsByTagName("last_position").at(0).toElement().attribute("timecode"),
                                                PhTimeCodeType25);


    //With DetX files, fps is always 25 so drop is false
    _fps = 25.00;
    _drop = false;
    _timeScale = 25.00;


    //Find the actors
    QDomNodeList charList = DetX->elementsByTagName("role");
    for (int i=0; i < charList.length(); i++)
    {
        QDomElement chara = charList.at(i).toElement();
        PhPeople *people = new PhPeople(chara.attribute("name"), PhColor(chara.attribute("color")));

        //Currently using id as key instead of name
        _actors[chara.attribute("id")] = people;
    }

    //Find the cut list
    QDomNodeList shotList = DetX->elementsByTagName("shot");
    for (int i=0; i < shotList.length(); i++)
    {
        _cuts.push_front(new PhStripCut(PhStripCut::Simple , PhTimeCode::frameFromString(shotList.at(i).toElement().attribute("timecode"),
                                                                                         PhTimeCodeType25)));
    }

    //Find the text list
    QDomNodeList lineList = DetX->elementsByTagName("line");
    for (int i=0; i < lineList.length(); i++)
    {
        QDomNode currentLine = lineList.at(i);
        PhString id = currentLine.toElement().attribute("role");
        for(int j = 0; j < currentLine.childNodes().length(); j++){

            if(currentLine.childNodes().at(j).nodeName() == "text"){
                if (!currentLine.childNodes().at(j+1).isNull()){
                    int start = PhTimeCode::frameFromString(currentLine.childNodes().at(j-1).toElement().attribute("timecode"), PhTimeCodeType25);
                    int end = PhTimeCode::frameFromString(currentLine.childNodes().at(j+1).toElement().attribute("timecode"), PhTimeCodeType25);
                    // if the sentence is short enough
                    if( end - start < 150)
                    {
                        _texts.push_back(new PhStripText(_actors[id],
                                                         currentLine.childNodes().at(j).toElement().text(),
                                                         start,
                                                         end,
                                                         currentLine.toElement().attribute("track").toInt(),
                                                         (j==1) ));
                    }
                    else // we split in half
                    {
                        int length = currentLine.childNodes().at(j).toElement().text().length();
                        PhString firstHalf = currentLine.childNodes().at(j).toElement().text().left(length/2);
                        PhString secondHalf = currentLine.childNodes().at(j).toElement().text().right(length/2);
                        //first split
                        _texts.push_back(new PhStripText(_actors[id],
                                                         firstHalf,
                                                         start,
                                                         start + (end - start)/2,
                                                         currentLine.toElement().attribute("track").toInt(),
                                                         (j==1) ));
                        //second split
                        _texts.push_back(new PhStripText(_actors[id],
                                                         secondHalf,
                                                         start + (end - start)/2,
                                                         end,
                                                         currentLine.toElement().attribute("track").toInt(),
                                                         false ));

                    }

                }
                else
                {
                    _texts.push_back(new PhStripText(_actors[id],
                                                     currentLine.childNodes().at(j).toElement().text(),
                                                     PhTimeCode::frameFromString(currentLine.childNodes().at(j-1).toElement().attribute("timecode"),
                                                                                 PhTimeCodeType25),
                                                     NULL,
                                                     currentLine.toElement().attribute("track").toInt(),
                                                     (j==1)));
                }
            }
        }
    }

    return true;
}

PhString PhStripDoc::getVideoPath(){
    return _videoPath;
}

int PhStripDoc::getDuration()
{
    return (_texts.last()->getTimeOut() - _videoTimestamp) / _fps;
}
PhString PhStripDoc::getTitle(){
    return _title;
}

PhTime PhStripDoc::getVideoTimestamp(){
    return _videoTimestamp;
}

PhTime PhStripDoc::getLastPosition(){
    return _lastPosition;
}

float PhStripDoc::getFps()
{
    return _fps;
}


bool PhStripDoc::getDrop()
{
    return _drop;
}

QMap<QString, PhPeople *> PhStripDoc::getActors()
{
    return _actors;
}



QList<PhStripText *> PhStripDoc::getTexts()
{
    return _texts;
}


/*
void Xml_Dom::statistics(){
    this->w->addMsg("-----Statistics-----------------");
    this->w->addMsg("Nombre de changement de plan : " + QString::number(detxDoc.elementsByTagName("shot").length()));
    this->w->addMsg("Nombre de roles : " + QString::number(detxDoc.elementsByTagName("role").length()));
}

void Xml_Dom::charList(){
    // get character list
    QDomNodeList charList = detxDoc.elementsByTagName("role");
    this->w->addMsg("-----CharList----------------");
    for (int i=0; i < 10; i++){
        this->w->addMsg("Char name : " + (charList.at(i).toElement().attribute("name")));
    }

    // display all characters


}
void Xml_Dom::showScript(){
    this->w->addMsg("----Script------------------");
    QDomNodeList lines = detxDoc.elementsByTagName("line");
    this->w->addMsg("Il y a " + QString::number(lines.length()) + " répliques");

    for (int i = 0; i<20; i++){
        QString txt = "";

        QDomNodeList line = lines.at(i).toElement().elementsByTagName("text");
        for (int j = 0; j < line.length(); j++){
            txt += (line.at(j).toElement().text());

        }
        this->w->addMsg(lines.at(i).toElement().attribute("role") + ": " + txt);
    }
}
*/
