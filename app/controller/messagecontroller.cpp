/**
  @file
  @author Stefan Frings
*/

#include "process.h"
#include "messagecontroller.h"

log4cpp::Category * MessageController::log4cpp = &log4cpp::Category::getInstance("MessageController");

MessageController::MessageController()
{
    log4cpp->debug("constructeur");
    QObject::connect( this,SIGNAL(signal_display_message(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)),&process_thread.process, SLOT(display_message(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));
}

void MessageController::service(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("service");
    QByteArray method=request.getMethod();
    QByteArray postheader=request.getHeader("Content-Type");
    QByteArray getheader=request.getHeader("Accept");
    response.setHeader("Content-Type", getheader);

    if (method=="GET")
    {
        if (getheader.contains("application/xml")) readXml_message(request,response);
    }
    else if (method=="POST")
    {
        if (postheader.contains("application/xml")) parseXml_message(request.getBody(),response);
    }
    QObject::disconnect( this,SIGNAL(signal_display_message(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)),&process_thread.process, SLOT(display_message(QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString,QString)));

}

void MessageController::readXml_message(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("readXml_message");
    QDomDocument doc;

    // Création de l'en-tête du fichier XML : <?xml version="1.0" encoding="UTF-8"?>
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    // On insère cet en-tête avant le premier enfant
    doc.insertBefore(xmlNode, doc.firstChild());

    // Création de l'élément <root>
    QDomElement root = doc.createElement("root");

    // On ajoute l'élément <root> en tant que premier enfant de notre document
    doc.appendChild(root);

    QDomElement message_1 = doc.createElement("message1");
    QDomElement titre_1 = doc.createElement("nom");
    QDomElement texte_1 = doc.createElement("nom");
    root.appendChild(message_1);
    message_1.appendChild(titre_1);
    titre_1.setAttribute("key","titre");
    titre_1.setAttribute( "value","Visite Médicale Mercredi 21 Décembre  à 18h50");
    message_1.appendChild(texte_1);
    texte_1.setAttribute("key","texte");
    texte_1.setAttribute( "value","Veuillez vous présenter avec votre carnet de santé");


    QDomElement message_2 = doc.createElement("message2");
    QDomElement titre_2 = doc.createElement("nom");
    QDomElement texte_2 = doc.createElement("nom");
    QDomElement texte_2_1 = doc.createElement("nom");
    root.appendChild(message_2);
    message_2.appendChild(titre_2);
    titre_2.setAttribute("key","titre");
    titre_2.setAttribute( "value","MESSAGE METIER");
    message_2.appendChild(texte_2);
    texte_2.setAttribute("key","texte");
    texte_2.setAttribute( "value","Nombre de jours en anomalies 21");
    message_2.appendChild(texte_2_1);
    texte_2_1.setAttribute("key","texte1");
    texte_2_1.setAttribute( "value","Veuillez diminuer ce nombre d'anomalies");

    QDomElement message_3 = doc.createElement("message3");
    QDomElement titre_3 = doc.createElement("nom");
    QDomElement texte_3 = doc.createElement("nom");
    QDomElement texte_3_1 = doc.createElement("nom");
    QDomElement compteur_3_1 = doc.createElement("nom");
    QDomElement compteur_3_2 = doc.createElement("nom");
    QDomElement compteur_3_3 = doc.createElement("nom");
    QDomElement compteur_3_4 = doc.createElement("nom");
    root.appendChild(message_3);
    message_3.appendChild(titre_3);
    titre_3.setAttribute("key","titre");
    titre_3.setAttribute( "value","COMPTEURS TEMPS REEL");
    message_3.appendChild(texte_3);
    texte_3.setAttribute("key","texte");
    texte_3.setAttribute( "value","Michel HAGET");
    message_3.appendChild(texte_3_1);
    texte_3_1.setAttribute("key","texte1");
    texte_3_1.setAttribute( "value","123456      12/01/20xx");
    message_3.appendChild(compteur_3_1);
    compteur_3_1.setAttribute("key","1");
    compteur_3_1.setAttribute( "value","D/C: 1h00");
    message_3.appendChild(compteur_3_2);
    compteur_3_2.setAttribute("key","2");
    compteur_3_2.setAttribute( "value","CP S: 15j00");
    message_3.appendChild(compteur_3_3);
    compteur_3_3.setAttribute("key","3");
    compteur_3_3.setAttribute( "value","RTTE: 3J");
    message_3.appendChild(compteur_3_4);
    compteur_3_4.setAttribute("key","4");
    compteur_3_4.setAttribute( "value","RTTS: 8J");

    QDomElement aux = doc.createElement("aux");
    QDomElement badge = doc.createElement("nom");
    QDomElement audio = doc.createElement("nom");
    QDomElement tempo = doc.createElement("nom");
    root.appendChild(aux);
    aux.appendChild(badge);
    badge.setAttribute("key","badge");
    badge.setAttribute( "value","");
    aux.appendChild(audio);
    audio.setAttribute("key","audio");
    audio.setAttribute( "value","");
    aux.appendChild(tempo);
    tempo.setAttribute("key","tempo");
    tempo.setAttribute( "value","30");

    response.write(doc.toByteArray());
    doc.clear();
}



void MessageController::parseXml_message(const QByteArray &data,HttpResponse& response)
{
    log4cpp->debug("parseXml_message");
    QString titre1; QString texte1;
    QString titre2;QString texte2;QString texte2_1;
    QString titre3;QString texte3;QString texte3_1;QString compteur1;QString compteur2;;QString compteur3;QString compteur4;
    QString badge;QString audio;QString tempo;
    QString null,*qstr;

    QDomDocument doc;
    // QDomDocument* doc = new QDomDocument();
    // Ajoute le contenu du fichier XML dans un QDomDocument et dit au QDomDocument de ne pas tenir compte des namespaces
    doc.setContent(data, false);

    // Ici, racine pointe sur l'élément <root> de notre document
    QDomElement racine = doc.documentElement();

    response.setStatus(464,"MESSAGE not set");

    if (!racine.tagName().compare("root"))
    {
        racine = racine.firstChildElement();

        //Boucle permettant la navigation dans le fichier XML
        // Ici, racine pointe sur un fils de <setup> c'est à dire <Message>
        while(!racine.isNull())
        {
            if(racine.tagName() == "message1")
            {

                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=&null;
                    // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("titre")) qstr=&titre1;
                    if (!elem.attribute("key").compare("texte")) qstr=&texte1;

                    *qstr=elem.attribute("value");
                    elem = elem.nextSiblingElement();
                }
            }
            if(racine.tagName() == "message2")
            {
                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=&null;
                    // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("titre")) qstr=&titre2;
                    if (!elem.attribute("key").compare("texte")) qstr=&texte2;
                    if (!elem.attribute("key").compare("texte1")) qstr=&texte2_1;

                    *qstr=elem.attribute("value");
                    elem = elem.nextSiblingElement();
                }
            }

            if(racine.tagName() == "message3")
            {
                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=&null;
                    // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("titre")) qstr=&titre3;
                    if (!elem.attribute("key").compare("texte")) qstr=&texte3;
                    if (!elem.attribute("key").compare("texte1")) qstr=&texte3_1;
                    if (!elem.attribute("key").compare("1")) qstr=&compteur1;
                    if (!elem.attribute("key").compare("2")) qstr=&compteur2;
                    if (!elem.attribute("key").compare("3")) qstr=&compteur3;
                    if (!elem.attribute("key").compare("4")) qstr=&compteur4;

                    *qstr=elem.attribute("value");
                    elem = elem.nextSiblingElement();
                }
            }

            if(racine.tagName() == "aux")
            {

                QDomElement elem = racine.firstChildElement();

                while(!elem.isNull())
                {
                    qstr=&null;
                    // On récupère le texte de l'élément <nom>
                    if (!elem.attribute("key").compare("badge")) qstr=&badge;
                    if (!elem.attribute("key").compare("audio")) qstr=&audio;
                    if (!elem.attribute("key").compare("tempo")) qstr=&tempo;
                    *qstr=elem.attribute("value");
                    elem = elem.nextSiblingElement();
                }
            }
            racine = racine.nextSiblingElement();
        }
        signal_display_message(titre1,texte1,titre2,texte2,texte2_1,titre3,texte3,texte3_1,compteur1,compteur2,compteur3,compteur4,badge,audio,tempo);
        response.setStatus(200,"OK");
        response.write("MESSAGE Affiché");
    }
    doc.clear();
}

