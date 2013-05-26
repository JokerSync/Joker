

#include "phstripdoc.h"


PhStripDoc::PhStripDoc(QString filename)
{
    openDetX(filename);
    exit(0);
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
        return true;
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
    //_videoTimestamp = DetX->elementsByTagName("videofile").at(0).toElement().attribute("timestamp");
    //while PhTime is just a float
    _videoTimestamp = 1.12768;

    //With DetX files, fps is always 25
    _fps = 25.00;
    _drop = false;

    /*
    _timeScale;
    */
    //PhPeople *test = new PhPeople("Name","Color");

    //Find the actors
    QDomNodeList charList = DetX->elementsByTagName("role");

    PhPeople *_actors = new PhPeople[charList.length()];
    for (int i=0; i < charList.length(); i++){
        _actors[i] = PhPeople(charList.at(i).toElement().attribute("name"),
                              charList.at(i).toElement().attribute("color"));
    }


    qDebug() << "Title : " << _title;
    qDebug() << "VideoPath : " << _videoPath;
    qDebug() << "VideoTimeStamp : " << _videoTimestamp;
    qDebug() << "FrameRate : " << _fps;
    qDebug() << "DropFrame : " << _drop;
    qDebug() << "characters list:";
    for (int i=0; i < charList.length(); i++){
        qDebug() << "name : " << _actors[i].getName() << " color: " << _actors[i].getColor() ;
    }
    return true;
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
