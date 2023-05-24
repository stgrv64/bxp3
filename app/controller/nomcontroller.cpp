/**
  @file
  @author Stefan Frings
*/

#include "nomcontroller.h"
#include "process.h"
#include "supervision.h"

#include <QVariant>
#include <QDateTime>
#include <QtSql>
#include <QtXml>
//#include <QtNetwork>
#include "x7gestconfig.h"

log4cpp::Category * NomController::log4cpp = &log4cpp::Category::getInstance("NomController");

NomController::NomController()
{
    log4cpp->debug("constructeur");
    QObject::connect( this,SIGNAL(signal_set_nom(QString)),X7GestConfig::getInstance(), SLOT(setHostName(QString)));
    QObject::connect( this,SIGNAL(signal_X7GestConfig_set_network(QString,QString,QString,QString)),X7GestConfig::getInstance(), SLOT(setIpV4Config(QString,QString,QString,QString)));
    QObject::connect( this,SIGNAL(signal_X7GestConfig_setWifiConfig(QString,QString,QString,QString)),X7GestConfig::getInstance(), SLOT(setWifiConfig(QString,QString,QString,QString)));
  }

void NomController::service(HttpRequest& request, HttpResponse& response)
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
             if (getheader.contains("application/xml")) readXml_nom(request,response);
            }

         else if (method=="POST")
                 {
                 if (postheader.contains("application/xml")) parseXml_nom(request.getBody(),response);
                 }
     QObject::disconnect( this,SIGNAL(signal_set_nom(QString)),X7GestConfig::getInstance(), SLOT(setHostName(QString)));
     QObject::disconnect( this,SIGNAL(signal_X7GestConfig_set_network(QString,QString,QString,QString)),X7GestConfig::getInstance(), SLOT(setIpV4Config(QString,QString,QString,QString)));
     QObject::disconnect( this,SIGNAL(signal_X7GestConfig_setWifiConfig(QString,QString,QString,QString)),X7GestConfig::getInstance(), SLOT(setWifiConfig(QString,QString,QString,QString)));

}



void NomController::readXml_nom(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("readXml_nom");
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





        //Création des l'élément
        QDomElement firmware = doc.createElement("firmware");
        QDomElement version = doc.createElement("nom");

        root.appendChild(firmware);
        firmware.appendChild(version);

        version.setAttribute("key","version");
        version.setAttribute( "value",supervision_thread.supervision.version_logiciel);

        //Création de l'élément <hostname>
        QDomElement hostname = doc.createElement("hostname");
        QDomElement name = doc.createElement("nom");

        root.appendChild(hostname);
        hostname.appendChild(name);

        name.setAttribute("key","name");
        name.setAttribute( "value", supervision_thread.supervision.time2bii_name);




        QHostAddress ip0,netmask0,gateway0;
        QString interface_name;
        QString MacAdress;
        QString a= QString("ip route");
        QProcess *qProc = new QProcess(this);
        qProc->start(a);
        qProc->waitForReadyRead();
        QString tmp = qProc->readAll();
        qProc->close();
        qProc->deleteLater();


        /// modif 1.6
        ///if tmp.contains("default via");

        gateway0=tmp.section(' ',2,2);
        QString gateways=gateway0.toString();
        if (gateways.isEmpty())
        {
            QFile in("/etc/network/interfaces");
            if (in.open( QFile::ReadOnly))
            {
                QTextStream stream_in(&in);
                QString s=stream_in.readAll();
                gateway0=s.section("gateway ",1,1);
                in.close();
            }

        }
        ip0.clear();
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
                         ip0 = entry.ip();
                         netmask0 =entry.netmask();
                         }
             }
        }

        // test mode dhcp ou statique
/*            QFileInfo in("/etc/network/if-up.d/udhcp");
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



        //Création de l'élément <ip>
        QDomElement ip = doc.createElement("ip");

        // On ajoute l'élément <horodate> en tant qu'enfant de l'élément <root>
        root.appendChild(ip);

        // Création des l'éléments <nom>
        QDomElement interface = doc.createElement("nom");
        QDomElement adresse = doc.createElement("nom");
        QDomElement netmask = doc.createElement("nom");
        QDomElement gateway = doc.createElement("nom");
        QDomElement macadress = doc.createElement("nom");
        QDomElement dnsa = doc.createElement("nom");
        QDomElement dnsb = doc.createElement("nom");

        interface.setAttribute("key","interface");
        adresse.setAttribute("key","adresse");
        netmask.setAttribute("key","netmask");
        gateway.setAttribute("key","gateway");
        macadress.setAttribute("key","MacAdress");
        dnsa.setAttribute("key","dns1");
        dnsb.setAttribute("key","dns2");

        interface.setAttribute( "value",interface_name);
        adresse.setAttribute( "value",ip0.toString());
        netmask.setAttribute( "value",netmask0.toString());
        gateway.setAttribute( "value",gateways);
        macadress.setAttribute( "value",MacAdress);
        dnsa.setAttribute("value",dns1);
        dnsb.setAttribute("value",dns2);

        // On ajoute l'élément <nom> en tant qu'enfant de l'élément <horodate>
        ip.appendChild(interface);
        ip.appendChild(adresse);
        ip.appendChild(netmask);
        ip.appendChild(gateway);
        ip.appendChild(macadress);
        ip.appendChild(dnsa);
        ip.appendChild(dnsb);


        response.write(doc.toByteArray());
        doc.clear();

  }



void NomController::parseXml_nom(const QByteArray &data, HttpResponse& response)
{
    log4cpp->debug("parseXml_nom");
    log4cpp->debug("parseXml_nom : " + data);
    QDomDocument doc;
      // Ajoute le contenu du fichier XML dans un QDomDocument et dit au QDomDocument de ne pas tenir compte des namespaces
      doc.setContent(data, false);

      // Ici, racine pointe sur l'élément <root> de notre document
       QDomElement racine = doc.documentElement();

       QString r2("Non modifié");
       QString r3("");
      response.setStatus(464,"Hostane ou IP not set");

      if (!racine.tagName().compare("root"))
          {
          racine = racine.firstChildElement();

          //Boucle permettant la navigation dans le fichier XML
          // Ici, racine pointe sur un fils de <setup> c'est à dire <badge>
           while(!racine.isNull())
              {
               if(racine.tagName() == "hostname")
                {
                   log4cpp->debug("hostname");
                    QString null,*qstr;
                    QString nom;
                    QDomElement elem = racine.firstChildElement();

                    while(!elem.isNull())
                    {
                     qstr=&null;
                    // On récupère le texte de l'élément <nom>
                         if (!elem.attribute("key").compare("name")) qstr=&nom;

                        *qstr=elem.attribute("value");
                      elem = elem.nextSiblingElement();
                    }
                    signal_set_nom(nom);
                    log4cpp->debug("hostname : " + nom.toStdString());
                    r2.clear();r2.append("Modifié: ");
                    r3.append("Hostname ");
                    response.setStatus(200,"OK");
                }

               if(racine.tagName() == "ip")
                {
                   log4cpp->debug("ip");
                   QString null,*qstr;
                   QString interface;QString adresse;QString netmask;QString gateway;
                   //QString MacAdress;
                    QDomElement elem = racine.firstChildElement();

                    while(!elem.isNull())
                    {
                        qstr=&null;
                       // On récupère le texte de l'élément <nom>
                        if (!elem.attribute("key").compare("interface")) qstr=&interface;
                        if (!elem.attribute("key").compare("adresse")) qstr=&adresse;
                        if (!elem.attribute("key").compare("netmask")) qstr=&netmask;
                        if (!elem.attribute("key").compare("gateway")) qstr=&gateway;
    //                    if (!elem.attribute("key").compare("MacAdress")) qstr=&MacAdress;

                        *qstr=elem.attribute("value");
                      elem = elem.nextSiblingElement();
                    }
                    signal_X7GestConfig_set_network(interface,adresse,netmask,gateway);
                    log4cpp->debug("ip : " + interface.toStdString() + " " + adresse.toStdString() + " " + netmask.toStdString() + " " + gateway.toStdString());
                    // uniquement pour test JJF
                    QString ssid = "";
                    QString psk = "";
                    signal_X7GestConfig_setWifiConfig(ssid,psk);
                    r2.clear();r2.append("Modifié: ");
                    r3.append("IP ");
                    response.setStatus(200,"OK");
                }

               racine = racine.nextSiblingElement();
               }
          }
      QString r("HOSTNAME ou IP");
      r.append(r2+r3);
      response.write(r.toStdString().c_str());

      doc.clear();
}
