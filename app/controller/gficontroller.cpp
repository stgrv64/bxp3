/**
  @file
  @author Stefan Frings
*/

#include "gficontroller.h"
#include "process.h"

#include <QVariant>
#include <QtSql>
#include <QtXml>
//#include <QtNetwork>

log4cpp::Category * GfiController::log4cpp = &log4cpp::Category::getInstance("GfiController");

GfiController::GfiController()
{
    log4cpp->debug("constructeur");
}

void GfiController::service(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("service");
    QByteArray method=request.getMethod();
    QByteArray postheader=request.getHeader("Content-Type");
    QByteArray getheader=request.getHeader("Accept");
    response.setHeader("Content-Type",getheader);

    log4cpp->debug("Method=" + QString(method).toStdString());
    log4cpp->debug("Postheader=" + QString(postheader).toStdString());
    log4cpp->debug("Getheader=" + QString(getheader).toStdString());
    log4cpp->debug("Body=" + QString(request.getBody()).toStdString());

    if (method=="GET")
    {
        if (getheader.contains("application/xml"))
            readXml_gfi(request,response);
    }
    else if (method=="POST")
    {
        QByteArray path=request.getPath();
        QString spath=QString::fromLatin1(path.data());

        QString j=QDate::currentDate().toString(Qt::ISODate);
        QString h=QTime::currentTime().toString(Qt::TextDate);
        QString pwd=QString("%1%2").arg(h.section(":",0,0)).arg(h.section(":",1,1));
        QString passwd= QString((request.getParameter("Pswd")));
        response.setHeader("Content-Type",getheader);
        if (!pwd.compare(passwd))
        {
            if (postheader.contains("application/xml"))
                parseXml_gfi(request.getBody(),response);
        }
        else
        {
            response.setStatus(464,"Erreur");
            response.write("Execution commande interdite");
        }
    }
}



void GfiController::readXml_gfi(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("readXml_gfi");
    // Objet de base servant à la création de notre fichier XML
    QDomDocument doc;

    // Création de l'en-tête du fichier XML : <?xml version="1.0" encoding="UTF-8"?>
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    // On insère cet en-tête avant le premier enfant
    doc.insertBefore(xmlNode, doc.firstChild());

    // Création de l'élément <root>
    QDomElement root = doc.createElement("root");

    // On ajoute l'élément <root> en tant que premier enfant de notre document
    doc.appendChild(root);

    //Création de l'élément <commande>
    QDomElement hostname = doc.createElement("commande");

    // On ajoute l'élément <hostname> en tant qu'enfant de l'élément <root>
    root.appendChild(hostname);

    // Création des l'éléments <nom>
    QDomElement name = doc.createElement("nom");
    name.setAttribute( "value","./restart_shell");

    hostname.appendChild(name);

    // On ajoute ce texte à l'élément <nom>
    name.setAttribute("key","name");

    response.write(doc.toByteArray());
    doc.clear();
}



void GfiController::parseXml_gfi(const QByteArray &data,HttpResponse& response)
{
    log4cpp->debug("parseXml_gfi");
    QDomDocument doc;
    // Ajoute le contenu du fichier XML dans un QDomDocument et dit au QDomDocument de ne pas tenir compte des namespaces
    doc.setContent(data, false);

    // Ici, racine pointe sur l'élément <root> de notre document
    QDomElement racine = doc.documentElement();

    log4cpp->debug("doc " + doc.toString().toStdString());
    response.setStatus(464,"COMMANDE not set");
    if (!racine.tagName().compare("root"))
    {
        log4cpp->debug("racine:" + racine.tagName().toStdString());
        racine = racine.firstChildElement();
        log4cpp->debug("racine :" + racine.tagName().toStdString());
        QByteArray a;
        //Boucle permettant la navigation dans le fichier XML
        while(!racine.isNull())
        {
            if(racine.tagName() == "commande")
            {
                QString null,qstr;
                QString nom;
                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=null;                  // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("name"))
                        qstr=nom;

                    qstr=elem.attribute("value");
                    log4cpp->debug(" ? " + qstr.toStdString());

                    elem = elem.nextSiblingElement();
                }
                QProcess *qProc2 = new QProcess(this);
                qProc2->execute("/bin/bash", { "-c", qstr});
                qProc2->waitForReadyRead();
                a=qProc2->readAll();
                log4cpp->debug("commande " + QString(a).toStdString());

                qProc2->close();
                qProc2->deleteLater();
                response.setStatus(200,"OK");
            }
            racine = racine.nextSiblingElement();
        }
        response.write("COMMANDE Exécutée : " + a);
    }
    doc.clear();
}

