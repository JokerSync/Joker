/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/


#include "PhStripDoc.h"


PhStripDoc::PhStripDoc(QString filename)
{
    openDetX(filename);
}

QList<PhStripCut *> PhStripDoc::getCuts()
{
    return _cuts;
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
                int start = PhTimeCode::frameFromString(currentLine.childNodes().at(j-1).toElement().attribute("timecode"), PhTimeCodeType25);
                int end = 0;
                if (!currentLine.childNodes().at(j+1).isNull()){
                    end = PhTimeCode::frameFromString(currentLine.childNodes().at(j+1).toElement().attribute("timecode"), PhTimeCodeType25);
                }
                else
                {
                    // One char is ~1.20588 frame
                    end = start + currentLine.childNodes().at(j).toElement().text().length() * 1.20588 + 1;
                }
                splitText(_actors[id], start, end,
                          currentLine.childNodes().at(j).toElement().text(), currentLine.toElement().attribute("track").toInt(),
                          (j==1), 0 );
            }
        }
    }
    return true;
}

PhString PhStripDoc::getVideoPath(){
    return _videoPath;
}


void PhStripDoc::splitText(PhPeople * actor, PhTime start, PhTime end, PhString sentence, int track, bool alone, int i){

    if(sentence != " " && sentence != "" ){
        // if the sentence is short enough
        if( end - start < 150)
        {
            _texts.push_back(new PhStripText(actor, sentence,
                                             start, end,
                                             track, alone));
        }
        else // we split in half
        {
            int length = sentence.length();
            splitText(actor, start, start + (end - start)/2, sentence.left(length/2), track, (i==0), i);
            i++;
            if (length % 2 == 0){
                splitText(actor, start + (end - start)/2, end, sentence.right(length/2), track, (i==0), i);
            }
            else{
                splitText(actor, start + (end - start)/2, end, sentence.right(length/2 + 1), track, (i==0), i);
            }
        }
    }
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
