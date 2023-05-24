/**
  @file
  @author Stefan Frings
*/

#include "setupcontroller.h"
#include "process.h"

#include <QVariant>
#include <QDateTime>
#include <QtSql>
#include <QtXml>
//#include <QtNetwork>

log4cpp::Category * SetupController::log4cpp = &log4cpp::Category::getInstance("SetupController");

SetupController::SetupController()
{
    log4cpp->debug("constructeur");
    QObject::connect( this,SIGNAL(signal_process_set_lecteur_badge(QString,QString,QString,QString)),&process_thread.process, SLOT(process_set_lecteur_badge(QString,QString,QString,QString)));
    QObject::connect( this,SIGNAL(signal_process_set_ntp(QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_ntp(QString,QString,QString,QString)));
    QObject::connect( this,SIGNAL(signal_process_set_divers(QString,QString,QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_divers(QString,QString,QString,QString,QString,QString)));
    QObject::connect(this,SIGNAL(signal_process_set_http(QString,QString,QString,QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_http(QString,QString,QString,QString,QString,QString,QString)));
    QObject::connect( this,SIGNAL(signal_set_touches_fonction(QString,QString,QString,QString,QString)), &process_thread.process, SLOT(set_touches_fonction(QString,QString,QString,QString,QString)));
}

void SetupController::service(HttpRequest& request, HttpResponse& response) {

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
        if (getheader.contains("application/xml")) readXml_setup(request,response);
    }

    else if (method=="POST")
    {
        if (postheader.contains("application/xml")) parseXml_setup(request.getBody(),response);
    }
    QObject::disconnect( this,SIGNAL(signal_process_set_lecteur_badge(QString,QString,QString,QString)),&process_thread.process, SLOT(process_set_lecteur_badge(QString,QString,QString,QString)));
    QObject::disconnect( this,SIGNAL(signal_process_set_ntp(QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_ntp(QString,QString,QString,QString)));
    QObject::disconnect( this,SIGNAL(signal_process_set_divers(QString,QString,QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_divers(QString,QString,QString,QString,QString,QString)));
    QObject::disconnect(this,SIGNAL(signal_process_set_http(QString,QString,QString,QString,QString,QString,QString)), &process_thread.process, SLOT(process_set_http(QString,QString,QString,QString,QString,QString,QString)));
    QObject::disconnect( this,SIGNAL(signal_set_touches_fonction(QString,QString,QString,QString,QString)), &process_thread.process, SLOT(set_touches_fonction(QString,QString,QString,QString,QString)));


}



void SetupController::readXml_setup(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("readXml_setup");
    QString http_server;
    QStringList l;
    QSqlDatabase config_db = QSqlDatabase::addDatabase("QSQLITE","setup");
    config_db.setDatabaseName("/var/x7/etc/database_setup.db");
    int i=0;
    int k=0;

    if (config_db.open())
    {
        log4cpp->debug("SetupController: Lecture des tables badge, http, ntp, divers, touchesfonction");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM badge",config_db);
        if (model.rowCount())
        {
            l << model.record(0).value("lecteur").toString();
            l << model.record(0).value("option").toString();
            l << model.record(0).value("masque").toString();
            l << model.record(0).value("format").toString();
        }

        model.setQuery("SELECT * FROM http",config_db);
        if (model.rowCount())
        {
            http_server= model.record(0).value("serveur").toString();
            l << model.record(0).value("serveur").toString();
            l << model.record(0).value("port https").toString();
            l << model.record(0).value("port http").toString();
            l << model.record(0).value("user").toString();
            l << model.record(0).value("password").toString();
            l << model.record(0).value("service").toString();
            l << model.record(0).value("timeout").toString();
        }
        model.setQuery("SELECT * FROM ntp",config_db);
        if (model.rowCount())
        {
            l << model.record(0).value("serveur").toString();
            l << model.record(0).value("cadence").toString();
            l << model.record(0).value("timezone").toString();
            l << model.record(0).value("langue").toString();
        }

        model.setQuery("SELECT * FROM divers",config_db);
        if (model.rowCount())
        {
            l << model.record(0).value("ecran").toString();
            l << model.record(0).value("backlight N1").toString();
            l << model.record(0).value("delai N2").toString();
            l << model.record(0).value("backlight N2").toString();
            l << model.record(0).value("delai OFF").toString();
            l << model.record(0).value("sleep mode").toString();
        }

        model.setQuery("SELECT * FROM touchesfonction",config_db);
        //          nb_funct=model.rowCount();
        if (model.rowCount())
        {
            while (i < model.rowCount())
            {
                l << model.record(i).value("libelle").toString();
                l << model.record(i).value("activite").toString();
                l << model.record(i).value("url").toString();
                l << model.record(i).value("proxy").toString();
                l << model.record(i).value("tempo").toString();
                i++;
            }
        }


        config_db.close();
        model.clear();
        model.deleteLater();


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

        //Création de l'élément <badge>
        QDomElement badge = doc.createElement("badge");

        // On ajoute l'élément <badge> en tant qu'enfant de l'élément <root>
        root.appendChild(badge);

        // Création des l'éléments <nom>
        QDomElement lecteur= doc.createElement("nom");
        QDomElement option = doc.createElement("nom");
        QDomElement masque = doc.createElement("nom");
        QDomElement format = doc.createElement("nom");

        // Ajout de l'attibut "value" qui aura la valeur "xx" à l'élément <nom>
        lecteur.setAttribute("key","lecteur");
        option.setAttribute("key","option");
        masque.setAttribute("key","masque");
        format.setAttribute("key","format");

        lecteur.setAttribute( "value",l.at(k++));
        option.setAttribute( "value",l.at(k++));
        masque.setAttribute( "value",l.at(k++));
        format.setAttribute( "value",l.at(k++));

        // On ajoute l'élément <nom> en tant qu'enfant de l'élément <horodate>
        badge.appendChild(lecteur);
        badge.appendChild(option);
        badge.appendChild(masque);
        badge.appendChild(format);


        //Création de l'élément <http>
        QDomElement http = doc.createElement("http");

        // On ajoute l'élément <http> en tant qu'enfant de l'élément <root>
        root.appendChild(http);

        // Création des l'éléments <nom>
        QDomElement serveur= doc.createElement("nom");
        QDomElement port_https = doc.createElement("nom");
        QDomElement port_http = doc.createElement("nom");
        QDomElement user= doc.createElement("nom");
        QDomElement password = doc.createElement("nom");
        QDomElement service = doc.createElement("nom");
        QDomElement timeout = doc.createElement("nom");

        // Ajout de l'attibut "value" qui aura la valeur "xx" à l'élément <nom>
        serveur.setAttribute("key","serveur");
        port_https.setAttribute("key","port https");
        port_http.setAttribute("key","port http");
        user.setAttribute("key","user");
        password.setAttribute("key","password");
        service.setAttribute("key","service");
        timeout.setAttribute("key","timeout");

        serveur.setAttribute( "value",l.at(k++));
        port_https.setAttribute( "value",l.at(k++));
        port_http.setAttribute( "value",l.at(k++));
        user.setAttribute( "value",l.at(k++));
        password.setAttribute( "value",l.at(k++));
        service.setAttribute( "value",l.at(k++));
        timeout.setAttribute( "value",l.at(k++));

        // On ajoute l'élément <nom> en tant qu'enfant de l'élément <http>
        http.appendChild(serveur);
        http.appendChild(port_https);
        http.appendChild(port_http);
        http.appendChild(user);
        http.appendChild(password);
        http.appendChild(service);
        http.appendChild(timeout);




        //Création de l'élément <ntp>
        QDomElement ntp = doc.createElement("ntp");

        // On ajoute l'élément <ntp> en tant qu'enfant de l'élément <root>
        root.appendChild(ntp);

        // Création des l'éléments <nom>
        QDomElement serveurn= doc.createElement("nom");
        QDomElement cadence = doc.createElement("nom");
        QDomElement timezone = doc.createElement("nom");
        QDomElement langue = doc.createElement("nom");

        // Ajout de l'attibut "value" qui aura la valeur "xx" à l'élément <nom>
        serveurn.setAttribute("key","serveur");
        cadence.setAttribute("key","cadence");
        timezone.setAttribute("key","timezone");
        langue.setAttribute("key","langue");

        serveurn.setAttribute( "value",l.at(k++));
        cadence.setAttribute( "value",l.at(k++));
        timezone.setAttribute( "value",l.at(k++));
        langue.setAttribute( "value",l.at(k++));

        // On ajoute l'élément <nom> en tant qu'enfant de l'élément <ntp>
        ntp.appendChild(serveurn);
        ntp.appendChild(cadence);
        ntp.appendChild(timezone);
        ntp.appendChild(langue);




        //Création de l'élément <divers>
        QDomElement divers = doc.createElement("divers");

        // On ajoute l'élément <divers> en tant qu'enfant de l'élément <root>
        root.appendChild(divers);

        // Création des l'éléments <nom>
        QDomElement ecran= doc.createElement("nom");
        QDomElement bl1 = doc.createElement("nom");
        QDomElement delai = doc.createElement("nom");
        QDomElement bl2 = doc.createElement("nom");
        QDomElement delai_off = doc.createElement("nom");
        QDomElement sleep_mode = doc.createElement("nom");

        // Ajout de l'attibut "value" qui aura la valeur "xx" à l'élément <nom>

        ecran.setAttribute("key","ecran");
        bl1.setAttribute("key","backlight N1");
        delai.setAttribute("key","delai N2");
        bl2.setAttribute("key","backlight N2");
        delai_off.setAttribute("key","delai OFF");
        sleep_mode.setAttribute("key","sleep mode");

        ecran.setAttribute( "value",l.at(k++));
        bl1.setAttribute( "value",l.at(k++));
        delai.setAttribute( "value",l.at(k++));
        bl2.setAttribute( "value",l.at(k++));
        delai_off.setAttribute( "value",l.at(k++));
        sleep_mode.setAttribute( "value",l.at(k++));

        // On ajoute l'élément <nom> en tant qu'enfant de l'élément <divers>
        divers.appendChild(ecran);
        divers.appendChild(bl1);
        divers.appendChild(delai);
        divers.appendChild(bl2);
        divers.appendChild(delai_off);
        divers.appendChild(sleep_mode);

        //Création de l'élément <touches fonction>
        QDomElement touchesfonction,libelle,activite,url,proxy,tempo;

        while (i>0)
        {
            touchesfonction = doc.createElement("touchesfonction");


            // On ajoute l'élément <touchesfonction> en tant qu'enfant de l'élément <root>
            root.appendChild(touchesfonction);

            // Création des l'éléments <nom>
            libelle= doc.createElement("nom");
            activite= doc.createElement("nom");
            url= doc.createElement("nom");
            proxy= doc.createElement("nom");
            tempo= doc.createElement("nom");

            // Ajout de l'attibut "value" qui aura la valeur "xx" à l'élément <nom>
            libelle.setAttribute("key","libelle");
            activite.setAttribute("key","activite");
            url.setAttribute("key","url");
            proxy.setAttribute("key","proxy");
            tempo.setAttribute("key","tempo");

            libelle.setAttribute( "value",l.at(k++));
            activite.setAttribute( "value",l.at(k++));
            url.setAttribute( "value",l.at(k++));
            proxy.setAttribute( "value",l.at(k++));
            tempo.setAttribute("value",l.at(k++));

            // On ajoute l'élément <nom> en tant qu'enfant de l'élément <touchesfonction>
            touchesfonction.appendChild(libelle);
            touchesfonction.appendChild(activite);
            touchesfonction.appendChild(url);
            touchesfonction.appendChild(proxy);
            touchesfonction.appendChild(tempo);


            i--;

        }

        response.write(doc.toByteArray());
        doc.clear();
    }
    else
    {
        response.setStatus(564,"Erreur");
        response.write("Erreur sur ouverture database Setup");
    }

}



void SetupController::parseXml_setup(const QByteArray &data,HttpResponse& response)
{
    log4cpp->debug("parseXml_setup");
    int touches=0;
    QString r2("Non modifié");
    QString r3("");
    response.setStatus(464,"SETUP not set");

    QDomDocument doc;
    // Ajoute le contenu du fichier XML dans un QDomDocument et dit au QDomDocument de ne pas tenir compte des namespaces
    doc.setContent(data, false);

    // Ici, racine pointe sur l'élément <root> de notre document
    QDomElement racine = doc.documentElement();

    if (!racine.tagName().compare("root"))
        {
        racine = racine.firstChildElement();

        //Boucle permettant la navigation dans le fichier XML
        // Ici, racine pointe sur un fils de <setup> c'est à dire <badge>
        while(!racine.isNull())
        {
            // Ici, racine pointe sur un élément <contributeur>
            if(racine.tagName() == "badge")
            {
                QString null,*qstr;
                QString lecteur;QString option;QString format;QString masque;
                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=&null;
                // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("lecteur")) qstr=&lecteur;
                    if (!elem.attribute("key").compare("option")) qstr=&option;
                    if (!elem.attribute("key").compare("masque")) qstr=&masque;
                    if (!elem.attribute("key").compare("format")) qstr=&format;

                    *qstr=elem.attribute("value");
                  elem = elem.nextSiblingElement();
                }
               signal_process_set_lecteur_badge(lecteur,option,masque,format);
               r2.clear();r2.append("Modifié: ");
               r3.append("Badge ");
               response.setStatus(200,"OK");
            }


             if(racine.tagName() == "ntp")
            {
                QString null,*qstr;
                QString serveur;QString cadence;;QString timezone;QString langue;
                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=&null;
                // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("serveur")) qstr=&serveur;
                    if (!elem.attribute("key").compare("cadence")) qstr=&cadence;
                    if (!elem.attribute("key").compare("timezone")) qstr=&timezone;
                    if (!elem.attribute("key").compare("langue")) qstr=&langue;

                    *qstr=elem.attribute("value");
                  elem = elem.nextSiblingElement();
                }
               signal_process_set_ntp(serveur,cadence,timezone,langue);
               r2.clear();r2.append("Modifié: ");
               r3.append("Ntp ");
               response.setStatus(200,"OK");
            }


            if(racine.tagName() == "divers")
            {
                QString null,*qstr;
                QString ecran;QString bl1;QString delai;QString bl2;QString delai_off;QString sleep_mode;
                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=&null;
                // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("ecran")) qstr=&ecran;
                    if (!elem.attribute("key").compare("backlight N1")) qstr=&bl1;
                    if (!elem.attribute("key").compare("delai N2")) qstr=&delai;
                    if (!elem.attribute("key").compare("backlight N2")) qstr=&bl2;
                    if (!elem.attribute("key").compare("delai OFF")) qstr=&delai_off;
                    if (!elem.attribute("key").compare("sleep mode")) qstr=&sleep_mode;

                   *qstr=elem.attribute("value");
                  elem = elem.nextSiblingElement();
                }
    //           httpserver_thread.httpserver.signal_httpserver=0;
                signal_process_set_divers(ecran,bl1,delai,bl2,delai_off,sleep_mode);
                r2.clear();r2.append("Modifié: ");
                r3.append("Divers ");
                response.setStatus(200,"OK");
             }


            if(racine.tagName() == "http")
            {
                QString null,*qstr;
                QString serveur;QString https;QString http;QString service;QString user;QString password;QString timeout;
                QDomElement elem = racine.firstChildElement();

                if (timeout.toInt() <2 ) timeout="2";

                while(!elem.isNull())
                {
                    qstr=&null;
                // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("serveur")) qstr=&serveur;
                    if (!elem.attribute("key").compare("port https")) qstr=&https;
                    if (!elem.attribute("key").compare("port http")) qstr=&http;
                    if (!elem.attribute("key").compare("user")) qstr=&user;
                    if (!elem.attribute("key").compare("password")) qstr=&password;
                    if (!elem.attribute("key").compare("service")) qstr=&service;
                    if (!elem.attribute("key").compare("timeout")) qstr=&timeout;

                    *qstr=elem.attribute("value");
                  elem = elem.nextSiblingElement();
                }
    //           httpserver_thread.httpserver.signal_httpserver=0;
                signal_process_set_http(serveur,https,http,service,user,password,timeout);
                r2.clear();r2.append("Modifié: ");
                r3.append("Http ");
                response.setStatus(200,"OK");
             }




            if(racine.tagName() == "touchesfonction")
            {
                QString null,*qstr;
                QString libelle;QString activite;QString url;QString proxy;QString tempo;
                QDomElement elem = racine.firstChildElement();
                if (touches==0)
                {
                    signal_set_touches_fonction("DELETE","DELETE","DELETE","DELETE","DELETE");
                    touches=1;
                    r2.clear();r2.append("Modifié: ");
                    r3.append("Touchesfonction ");
                    response.setStatus(200,"OK");
                }

                while(!elem.isNull())
                {
                qstr=&null;
                // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("libelle")) qstr=&libelle;
                    if (!elem.attribute("key").compare("activite")) qstr=&activite;
                    if (!elem.attribute("key").compare("url")) qstr=&url;
                    if (!elem.attribute("key").compare("proxy")) qstr=&proxy;
                    if (!elem.attribute("key").compare("tempo")) qstr=&tempo;

                    *qstr=elem.attribute("value");
                  elem = elem.nextSiblingElement();
                }
                signal_set_touches_fonction(libelle,activite,url,proxy,tempo);
             }

            // On va à l'élément fils de <root> suivant
            racine = racine.nextSiblingElement();
            if (touches==1 && racine.tagName().compare("touchesfonction")){signal_set_touches_fonction("FIN","","","","");}
            }
        }
    QString r("SETUP ");
    r.append(r2+r3);
    response.write(r.toStdString().c_str());
    doc.clear();
    log4cpp->debug("response.write : " + r.toStdString());
}

