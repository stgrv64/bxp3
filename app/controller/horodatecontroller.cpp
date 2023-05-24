/**
  @file
  @author Stefan Frings
*/

#include "horodatecontroller.h"
#include "process.h"


#include <QVariant>
#include <QDateTime>
#include <QtXml>

log4cpp::Category * HorodateController::log4cpp = &log4cpp::Category::getInstance("HorodateController");

HorodateController::HorodateController()
{
    log4cpp->debug("constructeur");
    QObject::connect( this, SIGNAL( signal_set_date_heure(QString,QString,QString,QString,QString,QString)), &process_thread.process, SLOT(priv_set_date_heure(QString,QString,QString,QString,QString,QString)));
}

void HorodateController::service(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("service");
    QByteArray method=request.getMethod();
    QByteArray postheader=request.getHeader("Content-Type");
    QByteArray getheader=request.getHeader("Accept");
    response.setHeader("Content-Type",getheader);

    log4cpp->debug("method : " + method);
    log4cpp->debug("postheader : " + postheader);
    log4cpp->debug("getheader : " + getheader);
    if (method=="GET")
    {
        if (getheader.contains("application/xml")) readXml_horodate(request,response);
    }

    else if (method=="POST")
    {
        if (postheader.contains("application/xml")) parseXml_horodate(request.getBody(),response);
    }
    QObject::disconnect( this, SIGNAL( signal_set_date_heure(QString,QString,QString,QString,QString,QString)), &process_thread.process, SLOT(priv_set_date_heure(QString,QString,QString,QString,QString,QString)));
}



void HorodateController::readXml_horodate(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("readXml_horodate");
    QDomDocument doc;

    // Création de l'en-tête du fichier XML : <?xml version="1.0" encoding="UTF-8"?>
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    // On insère cet en-tête avant le premier enfant
    doc.insertBefore(xmlNode, doc.firstChild());

    // Création de l'élément <root>
    QDomElement root = doc.createElement("root");

    // On ajoute l'élément <root> en tant que premier enfant de notre document
    doc.appendChild(root);

    //Création de l'élément <horodate>
    QDomElement horodate = doc.createElement("horodate");

    // On ajoute l'élément <horodate> en tant qu'enfant de l'élément <root>
    root.appendChild(horodate);

    // Création des l'éléments <nom>
    QDomElement an = doc.createElement("nom");
    QDomElement mois = doc.createElement("nom");
    QDomElement jour = doc.createElement("nom");
    QDomElement heure = doc.createElement("nom");
    QDomElement minute = doc.createElement("nom");
    QDomElement seconde = doc.createElement("nom");


    an.setAttribute( "key","an");
    mois.setAttribute("key","mois");
    jour.setAttribute( "key","jour");
    heure.setAttribute( "key","heure");
    minute.setAttribute( "key","minute");
    seconde.setAttribute( "key","seconde");

    QString j=QDate::currentDate().toString(Qt::ISODate);
    QString h=QTime::currentTime().toString(Qt::TextDate);
    // Ajout de l'attibut "value" qui aura la valeur "xx" à l'élément <nom>
    an.setAttribute( "value",j.section("-",0,0));
    mois.setAttribute( "value",j.section("-",1,1));
    jour.setAttribute( "value",j.section("-",2,2));
    heure.setAttribute( "value",h.section(":",0,0));
    minute.setAttribute( "value",h.section(":",1,1));
    seconde.setAttribute( "value",h.section(":",2,2));

    // On ajoute l'élément <nom> en tant qu'enfant de l'élément <horodate>
    horodate.appendChild(an);
    horodate.appendChild(mois);
    horodate.appendChild(jour);
    horodate.appendChild(heure);
    horodate.appendChild(minute);
    horodate.appendChild(seconde);


    response.write(doc.toByteArray());
    doc.clear();
}

void HorodateController::parseXml_horodate(const QByteArray &data,HttpResponse& response)
{
    log4cpp->debug("parseXml_horodate");
    QDomDocument doc;
    // Ajoute le contenu du fichier XML dans un QDomDocument et dit au QDomDocument de ne pas tenir compte des namespaces
    doc.setContent(data, false);

    // Ici, racine pointe sur l'élément <root> de notre document
    QDomElement racine = doc.documentElement();
    response.setStatus(464,"HORODATE not set");



    if (!racine.tagName().compare("root"))
    {
        racine = racine.firstChildElement();

        //Boucle permettant la navigation dans le fichier XML
        // Ici, racine pointe sur un fils de <setup> c'est à dire <badge>
        while(!racine.isNull())
        {
            // Ici, racine pointe sur un élément <contributeur>
            if(racine.tagName() == "horodate")
            {
                QString an;QString mois;QString jour;QString heure;QString minute;QString seconde;
                QString null,*qstr;
                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=&null;
                    // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("an")) qstr=&an;
                    if (!elem.attribute("key").compare("mois")) qstr=&mois;
                    if (!elem.attribute("key").compare("jour")) qstr=&jour;
                    if (!elem.attribute("key").compare("heure")) qstr=&heure;
                    if (!elem.attribute("key").compare("minute")) qstr=&minute;
                    if (!elem.attribute("key").compare("seconde")) qstr=&seconde;

                    *qstr=elem.attribute("value");
                    elem = elem.nextSiblingElement();
                }

                signal_set_date_heure(an,mois,jour,heure,minute,seconde);
                response.setStatus(200,"OK");
                response.write("HORODATE Modifiée");
            }
            racine = racine.nextSiblingElement();
        }
    }
    doc.clear();
}
