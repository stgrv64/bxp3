/**
@file
@author Stefan Frings
*/

#include "supervision.h"
#include "formcontroller.h"
#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>
//#include <QNetworkInterface>
#include <QtSql>
#include <QFile>
#include "x7gestconfig.h"
#include "x7gestlecteur.h"
#include "x7carteio.h"

log4cpp::Category * FormController::log4cpp = &log4cpp::Category::getInstance("FormController");

FormController::FormController()
{
    log4cpp->debug("constructeur");

}

void FormController::service(HttpRequest& req, HttpResponse& resp)
{
    log4cpp->debug("service");
    log4cpp->debug("req.getPath() : " + req.getPath());
    int write_ok=0xff;
    bool setup = false;
    if (req.getPath().startsWith("/setup/ip")) {
        write_ok &= 0x01;
        setup = true;
    }
    if (req.getPath().startsWith("/setup/lecteur")) write_ok &= 0x02;
    if (req.getPath().startsWith("/setup/http")) write_ok &=0x04;
    if (req.getPath().startsWith("/setup/ntp")) write_ok &=0x08;
    if (req.getPath().startsWith("/setup/divers")) write_ok &=0x10;
    // Configuration Badge, Serveur HTTP, Touches Fonctions, Divers
    QString http_server;
    QStringList parameterList;
    //  QString badge_family,matricule_longueur;
    //  QString http_server,http_port_https,http_port_http,http_service,http_user,http_password;
    QSqlDatabase config_db = QSqlDatabase::addDatabase("QSQLITE","setup");
    config_db.setDatabaseName("/var/x7/etc/database_setup.db");
    int i=0;
    if (config_db.open())
    {
        log4cpp->debug("config_db.open()");
        supervisionlog->debug("FormController: Lecture des tables badge, http, ntp, divers, touchesfonction");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM badge",config_db);
        if (model.rowCount() && (write_ok & 0x02))
        {
            log4cpp->debug("model.rowCount() && (write_ok & 0x02)");
            parameterList << model.record(0).value("lecteur").toString();
            log4cpp->debug("lecteur : " + model.record(0).value("lecteur").toString().toStdString());
            parameterList << model.record(0).value("option").toString();
            log4cpp->debug("option : " + model.record(0).value("option").toString().toStdString());
            parameterList << model.record(0).value("masque").toString();
            log4cpp->debug("masque : " + model.record(0).value("masque").toString().toStdString());
            parameterList << model.record(0).value("format").toString();
            log4cpp->debug("format : " + model.record(0).value("format").toString().toStdString());
        }
        model.setQuery("SELECT * FROM http",config_db);
        if (model.rowCount()&& (write_ok & 0x04))
        {
            log4cpp->debug("model.rowCount() && (write_ok & 0x04)");
            http_server= model.record(0).value("serveur").toString();
            log4cpp->debug("http_server : " + http_server.toStdString());
            parameterList << model.record(0).value("serveur").toString();
            log4cpp->debug("server : " + model.record(0).value("serveur").toString().toStdString());
            parameterList << model.record(0).value("port https").toString();
            log4cpp->debug("port https : " + model.record(0).value("port https").toString().toStdString());
            parameterList << model.record(0).value("port http").toString();
            log4cpp->debug("port http : " + model.record(0).value("port http").toString().toStdString());
            parameterList << model.record(0).value("user").toString();
            log4cpp->debug("user : " + model.record(0).value("user").toString().toStdString());
            parameterList << model.record(0).value("password").toString();
            log4cpp->debug("password : " + model.record(0).value("password").toString().toStdString());
            parameterList << model.record(0).value("service").toString();
            log4cpp->debug("service : " + model.record(0).value("service").toString().toStdString());
            parameterList << model.record(0).value("timeout").toString();
            log4cpp->debug("timeout : " + model.record(0).value("timeout").toString().toStdString());
        }

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","supervison_setup");
        db.setDatabaseName("/var/x7/etc/database_setup.db");
        QSqlQuery query(db);
        query.exec(QString("DELETE from divers"));
        query.clear();
        query.finish();
        db.commit();
        query.prepare("INSERT INTO divers (ecran, 'backlight N1', 'delai N2', 'backlight N2', 'delai OFF', 'sleep mode')" "VALUES (:ecran, :bl1, :delai, :bl2, :delai_off, :sleep_mode);");
        query.bindValue(":ecran", "0");
        query.bindValue(":bl1", "10");
        query.bindValue(":delai", "300");
        query.bindValue(":bl2", "20");
        query.bindValue(":delai_off", "200");
        query.bindValue(":sleep_mode", "0");
        query.exec();
        query.finish();
        db.commit();
        db.close();

        model.setQuery("SELECT * FROM ntp",config_db);
        if (model.rowCount()&& (write_ok & 0x08))
        {
            log4cpp->debug("model.rowCount() && (write_ok & 0x08)");
            parameterList << model.record(0).value("serveur").toString();
            log4cpp->debug("server : " + model.record(0).value("serveur").toString().toStdString());
            parameterList << model.record(0).value("cadence").toString();
            log4cpp->debug("cadence : " + model.record(0).value("cadence").toString().toStdString());
            parameterList << model.record(0).value("timezone").toString();
            log4cpp->debug("timezone : " + model.record(0).value("timezone").toString().toStdString());
            parameterList << model.record(0).value("langue").toString();
            log4cpp->debug("langue : " + model.record(0).value("langue").toString().toStdString());
        }
        model.setQuery("SELECT * FROM divers",config_db);
        if (model.rowCount()&& (write_ok & 0x10))
        {
            log4cpp->debug("model.rowCount() && (write_ok & 0x10)");
            parameterList << model.record(0).value("ecran").toString();
            log4cpp->debug("ecran : " + model.record(0).value("ecran").toString().toStdString());
            parameterList << model.record(0).value("backlight N1").toString();
            log4cpp->debug("backlight N1 : " + model.record(0).value("backlight N1").toString().toStdString());
        }
        model.setQuery("SELECT * FROM touchesfonction",config_db);
        if (model.rowCount()&& (write_ok & 0xE0))
        {
            log4cpp->debug("model.rowCount() && (write_ok & 0xE0)");
            while (i < model.rowCount())
            {
                parameterList << model.record(i).value("libelle").toString();
                log4cpp->debug("libelle : " + model.record(i).value("libelle").toString().toStdString());
                parameterList << model.record(i).value("activite").toString();
                log4cpp->debug("activite : " + model.record(i).value("activite").toString().toStdString());
                parameterList << model.record(i).value("url").toString();
                log4cpp->debug("url : " + model.record(i).value("url").toString().toStdString());
                parameterList << model.record(i).value("proxy").toString();
                log4cpp->debug("proxy : " + model.record(i).value("proxy").toString().toStdString());
                parameterList << model.record(i).value("tempo").toString();
                log4cpp->debug("tempo : " + model.record(i).value("tempo").toString().toStdString());
                i++;
            }
        }
        config_db.close();
        model.clear();
        model.deleteLater();
    }
    QHostAddress ip,netmask,gateway;
    QString interface_name;
    QString MacAdress;
    QString bodyStart;
    QString a= QString("ip route");
    QProcess *qProc = new QProcess(this);
    qProc->start(a);
    qProc->waitForReadyRead();
    QString tmp = qProc->readAll();
    qProc->close();
    qProc->deleteLater();
    gateway=tmp.section(' ',2,2);
    ip.clear();
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach(const QNetworkInterface & interface, interfaces)
    {
        if (interface.name().contains("eth") || interface.name().contains("wlan") || interface.name().contains("ppp"))
        {
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            if (!entries.isEmpty())
            {
                interface_name=interface.name();
                MacAdress=interface.hardwareAddress();
                QNetworkAddressEntry entry = entries.first();
                ip = entry.ip();
                netmask =entry.netmask();
            }
        }
    }
    // test mode dhcp ou statique
    /*          QFileInfo in("/etc/network/if-up.d/udhcp");
if (in.permission(QFile::ExeOwner | QFile::ExeOwner |QFile::ExeGroup)) interface_name.append(" Dhcp");
else interface_name.append(" Static");
*/

    if (X7GestConfig::getInstance()->dhcp())
        interface_name.append(" Dhcp");
    else interface_name.append(" Static");

    QString dns1,dns2;
    QFile in0("/etc/resolv.conf");
    if (in0.open( QFile::ReadOnly))
    {
        QTextStream stream_in(&in0);
        QString s;
        int i=0;
        do{
            s = stream_in.readLine();
            if (s.contains("nameserver"))
            {
                if (i==0) dns1=s.section(' ',1,1);
                else      dns2=s.section(' ',1,1);
                i++;
            }
        } while (!s.isNull());
        in0.close();
    }
    QString j=QDate::currentDate().toString(Qt::ISODate);
    QString h=QTime::currentTime().toString(Qt::TextDate);
    QSqlDatabase badge_db = QSqlDatabase::addDatabase("QSQLITE","badge_form");
    badge_db.setDatabaseName("/var/x7/contents/database_badge.db");
    qint64 nb_badgeage=0;
    if (badge_db.open())
    {
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM badge",badge_db);
        if (write_ok & 0x01)
        {
            while (model.canFetchMore())
                model.fetchMore();
            nb_badgeage=model.rowCount();
        }
        badge_db.close();
        model.clear();
        model.deleteLater();
    }
    //     resp.setHeader("Content-Type", "text/html; charset=ISO-8859-1");
    resp.setHeader("Content-Type", "text/html;charset=utf-8");
    //      resp.write(200,true);
    bodyStart.clear();
    bodyStart =  tr ("<!DOCTYPE html>\
                     <html><head>\
                     <style>\
                     table, th, td{border:1px solid black;border-collapse:collapse;}\
                     th,td{text-align:center;}\
                     body {font-size:100%;}\
                     h2 {font-size:1.2em; color: #8492b3}\
                     table {width:470px}\
                     table.center { margin-left: auto;  margin-right: auto}\
                     table.titre { margin-left: auto;  margin-right: auto; border:0px solid black;border-collapse:collapse;}\
                     tr.titre { margin-left: auto;  margin-right: auto; border:0px solid black;border-collapse:collapse;}\
                     th.titre { margin-left: auto;  margin-right: auto; border:0px solid black;border-collapse:collapse;}\
                     th.category {color: #1978da}\
                     </style>\
                     </head>\
                     <body>");

                     if (write_ok & 0x01 && !setup)
    {
                         log4cpp->debug("write_ok & 0x01");
                         bodyStart +=  tr (" <table class=titre>\
                         <tr class=titre>\
                         <th class=titre>\
                         <h2>Version Logiciel: %1</h2>\
                         <h2>%2 %3</h2>\
                         <h2>Badgeages stock&eacutes: %4 </h2><hr/>\
                         <h2>Configuration R&eacute;seau</h2>\
                         </th>\
                         </tr>\
                         </table>\
                         <table class=center>\
                         <tr>\
                         <th class=category>Nom</th>\
                         <th class=category>Interface</th>\
                         <th class=category>Adresse</th>\
                         <th class=category>Netmask</th>\
                         </tr>\
                         <tr>\
                         <td>%5</td>\
                         <td>%6</td>\
                         <td>%7</td>\
                         <td>%8</td>\
                         </tr>\
                         </table> \
                         <table class=center>\
                         <tr>\
                         <th class=category>Gateway</th>\
                         <th class=category>Mac Adress</th>\
                         </tr>\
                         <tr>\
                         <td>%9</td>\
                         <td>%10</td>\
                         </tr>\
                         </table>\
                         <table class=center>\
                         <tr>\
                         <th class=category>Dns1</th>\
                         <th class=category>Dns2</th>\
                         </tr>\
                         <tr>\
                         <td>%11</td>\
                         <td>%12</td>\
                         </tr>\
                         </table>").arg(supervision_thread.supervision.version_logiciel).arg(j).arg(h).arg(nb_badgeage).arg(supervision_thread.supervision.time2bii_name).arg(interface_name).arg(ip.toString())
                         .arg(netmask.toString()).arg(gateway.toString()).arg(MacAdress).arg(dns1).arg(dns2);
                         //if (write_ok & 0x01)
                         //    resp.write(bodyStart.toUtf8(),true);
                     }
                     if (write_ok & 0x01 && setup)
    {
                         log4cpp->debug("write_ok & 0x01");
                         bodyStart +=  tr ("<table class=titre>\
                         <tr class=titre>\
         <th class=titre>\
         <h2>Configuration R&eacute;seau</h2>\
         </th>\
         </tr>\
         </table>\
                         <table class=center>\
                         <tr>\
                         <th class=category>Nom</th>\
                         <th class=category>Interface</th>\
                         <th class=category>Adresse</th>\
                         <th class=category>Netmask</th>\
                         </tr>\
                         <tr>\
                         <td>%1</td>\
                         <td>%2</td>\
                         <td>%3</td>\
                         <td>%4</td>\
                         </tr>\
                         </table> \
                         <table class=center>\
                         <tr>\
                         <th class=category>Gateway</th>\
                         <th class=category>Mac Adress</th>\
                         </tr>\
                         <tr>\
                         <td>%5</td>\
                         <td>%6</td>\
                         </tr>\
                         </table>\
                         <table class=center>\
                         <tr>\
                         <th class=category>Dns1</th>\
                         <th class=category>Dns2</th>\
                         </tr>\
                         <tr>\
                         <td>%7</td>\
                         <td>%8</td>\
                         </tr>\
                         </table>").arg(supervision_thread.supervision.time2bii_name).arg(interface_name).arg(ip.toString())
                         .arg(netmask.toString()).arg(gateway.toString()).arg(MacAdress).arg(dns1).arg(dns2);
                         //if (write_ok & 0x01)
                         //    resp.write(bodyStart.toUtf8(),true);
                     }

                     int k=0;
            if (write_ok & 0x02)
    {

            log4cpp->debug("write_ok & 0x02 ");
            if(!parameterList.isEmpty())
            bodyStart += tr("<table class=titre>\
                            <tr class=titre>\
            <th class=titre>\
            <h2>Configuration Badge</h2>\
            </th>\
            </tr>\
            </table>\
            <table class=center>\
            <tr>\
            <th class=category>Lecteur</th>\
            <th class=category>Option</th>\
            <th class=category>Masque</th>\
            <th class=category>Format</th>\
            </tr>\
            <tr>\
            <td>%1</td>\
            <td>%2</td>\
            <td>%3</td>\
            <td>%4</td>\
            </tr>\
            </table>").arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++));

            bodyStart += tr("<table class=titre>\
                            <tr class=titre>\
            <th class=titre>\
            <h2>Configuration Lecteur</h2>\
            </th>\
            </tr>\
            </table>\
            <table class=center>\
            <tr>\
            <th class=category>Version</th>\
            <th class=category>%1</th>\
            </tr>\
            </table>").arg(X7GestLecteur::getInstance()->getVersionLecteur());


            bodyStart += tr("<table class=titre>\
                            <tr class=titre>\
            <th class=titre>\
            <h2>Configuration Carte I/O</h2>\
            </th>\
            </tr>\
            </table>\
            <table class=center>\
            <tr>\
            <th class=category>Version</th>\
            <th class=category>%1</th>\
            </tr>\
            </table>").arg(X7CarteIO::getInstance()->getVersionCarteIO());

            // if (write_ok & 0x02)
            //resp.write(bodyStart.toUtf8(),true);
}
            if (write_ok & 0x04)
    {
            log4cpp->debug("write_ok & 0x04");
            if (!http_server.isEmpty())
    {
            if (supervision_thread.supervision.http_erreurs)
    {
            if(!parameterList.isEmpty())
            bodyStart += tr("<table class=titre>\
                            <tr class=titre>\
            <th class=titre>\
            <h2>Configuration HTTP</h2>\
            </th>\
            </tr>\
            </table>\
            <table  bgcolor='red'  class=center>\
            <tr>\
            <th class=category>Serveur</th>\
            <th class=category>Port Https</th>\
            <th class=category>Port Http</th>\
            <th class=category>User</th>\
            <th class=category>Password</th>\
            </tr>\
            <tr>\
            <td>%1</td>\
            <td>%2</td>\
            <td>%3</td>\
            <td>%4</td>\
            <td>%5</td>\
            </tr>\
            </table>\
            <table bgcolor='red'   class=center>\
            <tr>\
            <th class=category>Service</th>\
            <th class=category>Timeout</th>\
            </tr>\
            <tr>\
            <td>%6</td>\
            <td>%7</td>\
            </tr>\
            </table>").arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++));
}
            else
    {
            if(!parameterList.isEmpty())
            bodyStart += tr(   "<table class=titre>\
                               <tr class=titre>\
            <th class=titre>\
            <h2>Configuration HTTP</h2>\
            </th>\
            </tr>\
            </table>\
            <table bgcolor='#66FF66' class=center>\
            <tr>\
            <th class=category>Serveur</th>\
            <th class=category>Port Https</th>\
            <th class=category>Port Http</th>\
            <th class=category>User</th>\
            <th class=category>Password</th>\
            </tr>\
            <tr>\
            <td >%1</td>\
            <td>%2</td>\
            <td>%3</td>\
            <td>%4</td>\
            <td>%5</td>\
            </tr>\
            </table> \
            <table bgcolor='#66FF66' class=center>\
            <tr>\
            <th class=category>Service</th>\
            <th class=category>Timeout</th>\
            </tr>\
            <tr>\
            <td>%6</td>\
            <td>%7</td>\
            </tr>\
            </table>").arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++));
}}
            else
    {
            if(!parameterList.isEmpty())
            bodyStart += tr( "<table class=titre>\
                             <tr class=titre>\
            <th class=titre>\
            <h2>Configuration HTTP</h2>\
            </th>\
            </tr>\
            </table>\
            <table  class=center>\
            <tr>\
            <th class=category>Serveur</th>\
            <th class=category>Port Https</th>\
            <th class=category>Port Http</th>\
            <th class=category>User</th>\
            <th class=category>Password</th>\
            </tr>\
            <tr>\
            <td >%1</td>\
            <td>%2</td>\
            <td>%3</td>\
            <td>%4</td>\
            <td>%5</td>\
            </tr>\
            </table> \
            <table  class=center>\
            <tr>\
            <th class=category>Service</th>\
            <th class=category>Timeout</th>\
            </tr>\
            <tr>\
            <td>%6</td>\
            <td>%7</td>\
            </tr>\
            </table>").arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++));
}
}
            if (write_ok & 0x08)
    {
            log4cpp->debug("write_ok & 0x08");
            if(!parameterList.isEmpty())
            bodyStart+= tr("<table class=titre>\
                           <tr class=titre>\
            <th class=titre>\
            <h2>Configuration Ntp</h2>\
            </th>\
            </tr>\
            </table>\
            <table  class=center>\
            <tr>\
            <th class=category>Serveur</th>\
            <th class=category>Cadence</th>\
            </tr>\
            <tr>\
            <td>%1</td>\
            <td>%2</td>\
            </tr>\
            </table>\
            <table  class=center>\
            <tr>\
            <th class=category>Timezone</th>\
            <th class=category>Langue</th>\
            </tr>\
            <tr>\
            <td>%3</td>\
            <td>%4</td>\
            </tr>\
            </table>").arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++));
}
            if (write_ok & 0x10)
    {
            log4cpp->debug("write_ok & 0x10");
            log4cpp->debug("write_ok & 0x10 %d",parameterList.contains("backlight N1"));
          if(!parameterList.isEmpty()){
            bodyStart += tr("<table class=titre>\
                            <tr class=titre>\
            <th class=titre>\
            <h2>Configuration Divers</h2>\
            </th>\
            </tr>\
            </table>\
            <table class=center>\
            <tr>\
            <th class=category>Orientation &eacute;cran</th>\
            <th class=category>R&eacute;tro &eacute;clairage</th>\
            </tr>\
            <tr>\
            <td>%1</td>\
            <td>%2</td>\
            </tr>\
            </table>").arg(parameterList.at(k++)).arg(parameterList.at(k++)) ;
        }
}
            if (write_ok & 0xE0)
    {
            log4cpp->debug("write_ok & 0xE0");
            bodyStart += tr("<table class=titre>\
                            <tr class=titre>\
            <th class=titre>\
            <h2>Configuration Touches Fonctions</h2>\
            </th>\
            </tr>\
            </table>");
            int j=0;
            while (j<(4*i))
    {
            if(!parameterList.isEmpty())
            bodyStart += tr(  "<table  class=center>\
                              <tr>\
                              <th class=category>Libelle</th>\
            <th class=category>Activite</th>\
            <th class=category>Proxy</th>\
            <th class=category>Tempo</th>\
            </tr>\
            <tr>\
            <td>%1</td>\
            <td>%2</td>\
            <td>%4</td>\
            <td>%5</td>\
            </tr>\
            </table> \
            <table  class=center>\
            <tr>\
            <th class=category>Url</th>\
            </tr>\
            <tr>\
            <td>%3</td>\
            </tr>\
            </table>").arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)).arg(parameterList.at(k++)) ;
            j+=4;
}
            //resp.write(bodyStart.toUtf8(),true);
}
            bodyStart += tr("</body>\
                            </html>");
                            resp.write(bodyStart.toUtf8(),true);
}
