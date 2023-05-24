/**
  @file
  @author Stefan Frings
*/

#include "process.h"
#include "browsercontroller.h"

log4cpp::Category * BrowserController::log4cpp = &log4cpp::Category::getInstance("BrowserController");

BrowserController::BrowserController()
{
    log4cpp->debug("constructeur");
    qDebug() << "constructeur Browser controller";
    QObject::connect( this,SIGNAL(signal_process_browser(QString,QString,QString,QString)),&process_thread.process, SLOT(process_browser(QString,QString,QString,QString)));
}

void BrowserController::service(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("service");
    qDebug() << "service Browser controller";
    QByteArray method=request.getMethod();
    QByteArray postheader=request.getHeader("Content-Type");
    QByteArray getheader=request.getHeader("Accept");
    response.setHeader("Content-Type", getheader);

    if (method=="GET")
    {
        if (getheader.contains("application/xml")) readXml_browser(request,response);
    }
    else if (method=="POST")
    {
        if (postheader.contains("application/xml")) parseXml_browser(request.getBody(),response);
    }
    QObject::disconnect( this,SIGNAL(signal_process_browser(QString,QString,QString,QString)),&process_thread.process, SLOT(process_browser(QString,QString,QString,QString)));
}


void BrowserController::readXml_browser(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("readXml_browser");
    qDebug() << "readXml Browser controller";
   QDomDocument doc;

    // Création de l'en-tête du fichier XML : <?xml version="1.0" encoding="UTF-8"?>
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    // On insère cet en-tête avant le premier enfant
    doc.insertBefore(xmlNode, doc.firstChild());

    // Création de l'élément <root>
    QDomElement root = doc.createElement("root");

    // On ajoute l'élément <root> en tant que premier enfant de notre document
    doc.appendChild(root);

    //Création de l'élément <browser>
    QDomElement browser = doc.createElement("browser");

    // On ajoute l'élément <horodate> en tant qu'enfant de l'élément <root>
    root.appendChild(browser);

    // Création des l'éléments <nom>
    QDomElement url = doc.createElement("nom");
    QDomElement proxy = doc.createElement("nom");
    QDomElement audio = doc.createElement("nom");
    QDomElement tempo = doc.createElement("nom");

    url.setAttribute( "key","url");
    proxy.setAttribute( "key","proxy");
    audio.setAttribute("key","audio");
    tempo.setAttribute( "key","tempo");

    url.setAttribute( "value","http://127.0.0.1/setup");
    proxy.setAttribute( "value","");
    audio.setAttribute( "value","");
    tempo.setAttribute( "value","10");

    // On ajoute l'élément <nom> en tant qu'enfant de l'élément <horodate>
    browser.appendChild(url);
    browser.appendChild(proxy);
    browser.appendChild(audio);
    browser.appendChild(tempo);

    response.write(doc.toByteArray());
    doc.clear();
}

void BrowserController::parseXml_browser(const QByteArray &data, HttpResponse& response)
{
    log4cpp->debug("parseXml_browser");
    qDebug() << "parseXml Browser controller";
    QDomDocument doc;
    // Ajoute le contenu du fichier XML dans un QDomDocument et dit au QDomDocument de ne pas tenir compte des namespaces
    doc.setContent(data, false);

    // Ici, racine pointe sur l'élément <root> de notre document
    QDomElement racine = doc.documentElement();

    response.setStatus(464,"BROWSER not set");

    if (!racine.tagName().compare("root"))
    {
        racine = racine.firstChildElement();

        //Boucle permettant la navigation dans le fichier XML
        // Ici, racine pointe sur un fils de <setup> c'est à dire <browser>
        while(!racine.isNull())
        {
            if(racine.tagName() == "browser")
            {
                QString null,*qstr;
                QString url;QString proxy;QString audio;QString tempo;
                proxy.clear();
                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    // On récupère le texte de l'élément <nom>
                    qstr=&null;
                    if (!elem.attribute("key").compare("url")) qstr=&url;
                    if (!elem.attribute("key").compare("proxy")) qstr=&proxy;
                    if (!elem.attribute("key").compare("audio")) qstr=&audio;
                    if (!elem.attribute("key").compare("tempo")) qstr=&tempo;

                    *qstr=elem.attribute("value");
                    elem = elem.nextSiblingElement();
                }
                response.setStatus(200,"OK");
                response.write("BROWSER ok");
                signal_process_browser(url,proxy,audio,tempo);
            }
            racine = racine.nextSiblingElement();
        }
    }
    doc.clear();
}

