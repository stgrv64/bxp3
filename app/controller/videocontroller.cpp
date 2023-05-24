/**
  @file
  @author Stefan Frings
*/

#include "process.h"
#include "videocontroller.h"

log4cpp::Category * VideoController::log4cpp = &log4cpp::Category::getInstance("VideoController");

VideoController::VideoController()
{
    log4cpp->debug("constructeur");
    QObject::connect( this,SIGNAL(signal_video(QString,QString)),&process_thread.process, SLOT(video(QString,QString)));
}

void VideoController::service(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("service");

    QByteArray method=request.getMethod();
    QByteArray postheader=request.getHeader("Content-Type");
    QByteArray getheader=request.getHeader("Accept");
    response.setHeader("Content-Type", getheader);

    if (method=="GET")
    {
        if (getheader.contains("application/xml")) readXml_video(request,response);
    }
    else if (method=="POST")
    {
        if (postheader.contains("application/xml")) parseXml_video(request.getBody(),response);
    }
    QObject::disconnect( this,SIGNAL(signal_video(QString,QString)),&process_thread.process, SLOT(video(QString,QString)));

}


void VideoController::readXml_video(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("readXml_video");

    QDomDocument doc;

    // Création de l'en-tête du fichier XML : <?xml version="1.0" encoding="UTF-8"?>
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    // On insère cet en-tête avant le premier enfant
    doc.insertBefore(xmlNode, doc.firstChild());

    // Création de l'élément <root>
    QDomElement root = doc.createElement("root");

    // On ajoute l'élément <root> en tant que premier enfant de notre document
    doc.appendChild(root);

    //Création de l'élément <videor>
    QDomElement video = doc.createElement("video");

    // On ajoute l'élément <horodate> en tant qu'enfant de l'élément <root>
    root.appendChild(video);

    // Création des l'éléments <nom>
    QDomElement nom = doc.createElement("nom");
    QDomElement tempo = doc.createElement("nom");


    nom.setAttribute( "key","nom");
    tempo.setAttribute( "key","tempo");

    nom.setAttribute( "value","gfi.mp4");
    tempo.setAttribute( "value","60");

    // On ajoute l'élément <nom> en tant qu'enfant de l'élément <horodate>
    video.appendChild(nom);
    video.appendChild(tempo);


    response.write(doc.toByteArray());
    doc.clear();
}

void VideoController::parseXml_video(const QByteArray &data, HttpResponse& response)
{
    log4cpp->debug("parseXml_video");
    QDomDocument doc;
    // Ajoute le contenu du fichier XML dans un QDomDocument et dit au QDomDocument de ne pas tenir compte des namespaces
    doc.setContent(data, false);

    // Ici, racine pointe sur l'élément <root> de notre document
    QDomElement racine = doc.documentElement();

    response.setStatus(464,"VIDEO not set");

    if (!racine.tagName().compare("root"))
    {
        racine = racine.firstChildElement();

        //Boucle permettant la navigation dans le fichier XML
        // Ici, racine pointe sur un fils de <setup> c'est à dire <browser>
        while(!racine.isNull())
        {
            if(racine.tagName() == "video")
            {
                QString null,*qstr;
                QString nom;QString tempo;
                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=&null;
                    //On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("nom")) qstr=&nom;
                    if (!elem.attribute("key").compare("tempo")) qstr=&tempo;

                    *qstr=elem.attribute("value");
                    elem = elem.nextSiblingElement();
                }
                response.setStatus(200,"OK");
                response.write("VIDEO ok");
                signal_video(nom,tempo);
            }
            racine = racine.nextSiblingElement();
        }
    }
    doc.clear();
}

