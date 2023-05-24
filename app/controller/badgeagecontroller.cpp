/**
  @file
  @author Stefan Frings
*/

#include "badgeagecontroller.h"

#include <QVariant>
#include <QDateTime>
#include <QtSql>
#include <QtXml>

log4cpp::Category * BadgeageController::log4cpp = &log4cpp::Category::getInstance("BadgeageController");

BadgeageController::BadgeageController()
{
    log4cpp->debug("constructeur");
}

void BadgeageController::service(HttpRequest& request, HttpResponse& response)
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
        if (getheader.contains("application/xml")) readXml_badgeage(request,response);
    }
    else if (method=="DELETE")
    {
        //                 if (postheader == "application/xml") delete_badgeage(request,response);
        delete_badgeage(request,response);
    }
    /*        else if (method=="POST")
                 {
                 response.setHeader("Content-Type",postheader);
                 if (postheader == "application/xml") parseXml_badgeage(request.getBody(),response);
                 }
*/
}



void BadgeageController::readXml_badgeage(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("readXml_badgeage");
    QSqlDatabase badgeage_db = QSqlDatabase::addDatabase("QSQLITE","badgeage");
    badgeage_db.setDatabaseName("/var/x7/contents/database_badge.db");
    QStringList l;
    qint64 i=0;
    qint64 j=0;
    qint64 k=0;
    if (badgeage_db.open())
    {
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM badge",badgeage_db);
        while (model.canFetchMore()) model.fetchMore();
        j=model.rowCount();
        while (i < j)
        {
            l << model.record(i).value("matric").toString();
            l << model.record(i).value("heure").toString();
            l << model.record(i).value("jour").toString();
            l << model.record(i).value("activite").toString();
            l << model.record(i).value("key").toString();
            i++;
        }
        badgeage_db.close();
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


        //Création de l'élément <nombre>
        QDomElement nombre= doc.createElement("nombre");

        // On ajoute l'élément <nombre_badgeage> en tant qu'enfant de l'élément <root>
        root.appendChild(nombre);

        // Création des l'éléments <nom>
        QDomElement total = doc.createElement("nom");
        // On ajoute l'élément <nom> en tant qu'enfant de l'élément <nombre>
        total.setAttribute("key","total");
        total.setAttribute( "value",QString("%1").arg(i));

        nombre.appendChild(total);

        //Création de l'élément <item>
        QDomElement item, matric,heure,jour,activite,cle;

        while (i>0)
        {
            item = doc.createElement("item");
            root.appendChild(item);

            matric= doc.createElement("nom");
            heure= doc.createElement("nom");
            jour = doc.createElement("nom");
            activite = doc.createElement("nom");
            cle = doc.createElement("nom");

            matric.setAttribute("key","matric");
            heure.setAttribute("key","heure");
            jour.setAttribute("key","jour");
            activite.setAttribute("key","activite");
            cle.setAttribute("key","cle");

            matric.setAttribute( "value",l.at(k++));
            heure.setAttribute( "value",l.at(k++));
            jour.setAttribute( "value",l.at(k++));
            activite.setAttribute( "value",l.at(k++));
            cle.setAttribute( "value",l.at(k++));

            item.appendChild(matric);
            item.appendChild(heure);
            item.appendChild(jour);
            item.appendChild(activite);
            item.appendChild(cle);
            i--;
        }

        response.write(doc.toByteArray());
        doc.clear();
    }
    else response.setStatus(564,"Erreur sur ouverture database Badgeage");
}


void BadgeageController::delete_badgeage(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("delete_badgeage");
    QByteArray path=request.getPath();
    QString spath=QString::fromLatin1(path.data());

    QString j=QDate::currentDate().toString(Qt::ISODate);
    QString h=QTime::currentTime().toString(Qt::TextDate);
    QString pwd=QString("%1%2").arg(h.section(":",0,0)).arg(h.section(":",1,1));
    QString cle= QString((request.getParameter("Key")));
    QString passwd= QString((request.getParameter("Pswd")));

    if (!pwd.compare(passwd))
    {
        if (!cle.compare("complet"))
        {
            QSqlDatabase badgeage_db = QSqlDatabase::addDatabase("QSQLITE","badgeage");
            badgeage_db.setDatabaseName("/var/x7/contents/database_badge.db");

            if (badgeage_db.open())
            {
                QSqlQueryModel model;
                model.setQuery("Delete FROM badge",badgeage_db);
                badgeage_db.commit();
                model.setQuery("SELECT * FROM badge",badgeage_db);
                if (!model.rowCount())
                {
                    response.setStatus(200);
                    response.write("Tous les Badgeages sont supprimés");
                }
                else
                    response.setStatus(464,"Erreur sur suppression badgeages");
                badgeage_db.close();
                model.clear();
                model.deleteLater();
            }
            else response.setStatus(564,"Erreur sur ouverture database badgeage");
        }
        else
        {
            bool ok;
            if (cle.toULongLong(&ok,10))
            {
                QSqlDatabase badgeage_db = QSqlDatabase::addDatabase("QSQLITE","badgeage");
                badgeage_db.setDatabaseName("/var/x7/contents/database_badge.db");

                if (badgeage_db.open())
                {
                    QSqlQueryModel model;
                    model.setQuery("SELECT * FROM badge",badgeage_db);
                    qint64 modelcount=model.rowCount();

                    model.setQuery(QString("Delete FROM badge where key='%1'").arg(cle),badgeage_db);
                    badgeage_db.commit();
                    model.setQuery("SELECT * FROM badge",badgeage_db);
                    if (model.rowCount()== modelcount-1)
                    {
                        response.setStatus(200);
                        response.write("Badgeage supprimé");
                    }
                    else
                        response.setStatus(464,"Erreur sur suppression badgeage");
                    badgeage_db.close();
                    model.clear();
                    model.deleteLater();
                }
                else
                    response.setStatus(564,"Erreur sur ouverture database badgeage");
            }
            else
                response.setStatus(464,"Erreur Clé invalide");
        }
    }
    else response.setStatus(464,"Erreur Suppression Badgeage interdite");
}

/*
void BadgeageController::parseXml_badgeage(const QByteArray &data,HttpResponse& response)
{
    QDomDocument* doc = new QDomDocument();
    // Ajoute le contenu du fichier XML dans un QDomDocument et dit au QDomDocument de ne pas tenir compte des namespaces
    doc->setContent(data, false);

    // Ici, racine pointe sur l'élément <root> de notre document}
    QDomElement racine = doc->documentElement();

    if (!racine.tagName().compare("root"))
        {
        racine = racine.firstChildElement();

            while(!racine.isNull())
            {
                if(racine.tagName() == "eraseall")
                {
                    QSqlDatabase badgeage_db = QSqlDatabase::addDatabase("QSQLITE","badgeage");
                    badgeage_db.setDatabaseName("/var/x7/contents/database_badge.db");

                    if (badgeage_db.open())
                           {
                           QSqlQueryModel model;
                           model.setQuery("Delete FROM badge",badgeage_db);
                           badgeage_db.commit();
                           badgeage_db.close();
                           model.deleteLater();
                    }
                }

                if(racine.tagName() == "eraseitem")
                {
                    QString cle,*qstr;
                    QDomElement elem = racine.firstChildElement();
                    QSqlDatabase badgeage_db = QSqlDatabase::addDatabase("QSQLITE","badgeage");
                    badgeage_db.setDatabaseName("/var/x7/contents/database_badge.db");

                    if (badgeage_db.open())
                             {
                               QSqlQueryModel model;
                                while(!elem.isNull())
                                {
                                if (!elem.attribute("key").compare("cle"))
                                    {
                                     qstr=&cle;
                                     *qstr=elem.attribute("value");
                                     model.setQuery(QString("Delete FROM badge where key='%1'").arg(cle),badgeage_db);
                                     badgeage_db.commit();
                                     }
                                elem = elem.nextSiblingElement();
                                }

                            badgeage_db.close();
                            model.deleteLater();
                            }
                    }
                racine = racine.nextSiblingElement();
            }

        }
}
*/
