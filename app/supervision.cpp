/**
 * @file
 * @brief Supervision_qobject and Supervision_qthread class definition
*******************************************************************/
#include <process.h>
#include <supervision.h>
//#include <QDebug>
#include <QThread>
#include "T2bEvent.h"


// QOBJECT
//
// PRIVATE et PUBLIC FONCTIONS
//


Supervision_qobject * Supervision_qobject::instance = 0;

const QString Supervision_qobject::version_logiciel_i="Smartime 2.0.1";

log4cpp::Category *supervisionlog;

/**
 * @brief Supervision_qobject::Supervision_qobject
 */
Supervision_qobject::Supervision_qobject(void)
{
    supervisionlog = &log4cpp::Category::getInstance("supervision");
    supervisionlog->debug("constructeur");
}
/**
 * @brief Supervision_qobject::getInstance
 * @return
 */
Supervision_qobject * Supervision_qobject::getInstance()
{
    supervisionlog->debug("getInstance");
    if ( !Supervision_qobject::instance )
    {
        Supervision_qobject * supervision = new Supervision_qobject();
        Supervision_qobject::instance = supervision;
    }

    return Supervision_qobject::instance;
}

/**
 * @brief Supervision_qobject::supervision_init
 * - appelé par Supervision_qthread::run()
 * - appel
 */
void Supervision_qobject::supervision_init(void)
{
    supervisionlog->debug("supervision init");
    QTimer *timer_init =new QTimer();
    timer_init->setSingleShot(true);
    connect(timer_init,SIGNAL(timeout()),this,SLOT(supervision_init0()));
    timer_init->start(1);
}



/**
 * @brief init de l'objet
 * - creation trigger Stimer
 * - ceation badge_db /var/x7/contents/database_badge.db"
 * - creation manager pour gestion des network request et envoi de data
 * - creation request pour la network request
 * - appel priv_attente_ip() recherche adresse ip
 * - appel supervision_http_status() param -1 pour affichage ihm du logo alerte
 * - appel signal_process_init_http() lecture datatbase_setup.db et init infos ihm
 * - appel supervision_init_http() connexion serveur http/https (chronotime)
 */
void Supervision_qobject::supervision_init0(void)
{
    supervisionlog->debug("supervision_init0(void)");
    http_initok=0;
    idPost=0;
    Stimer =new QTimer();
    Stimer->setSingleShot(true);
    connect(Stimer,SIGNAL(timeout()),this,SLOT(priv_trigger()));

    manager=new QNetworkAccessManager(this) ;
    request= new QNetworkRequest();
    connect(manager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this,
            SLOT(provideAuthentication(QNetworkReply*,QAuthenticator*)));

    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(finishedSlot(QNetworkReply*)));
    timerloop.setSingleShot(true);

    // Creation database badge
    badge_db = QSqlDatabase::addDatabase("QSQLITE","supervision_badge");
    badge_db.setDatabaseName("/var/x7/contents/database_badge.db");
    if (badge_db.open())
    {
        QSqlQueryModel model;
        QSqlQuery query(badge_db);
        model.setQuery("SELECT * FROM badge",badge_db);
        if (!model.rowCount()) query.exec(QString("DROP TABLE badge"));
        query.exec(QString("CREATE TABLE badge (matric varchar(30),heure varchar(10),jour varchar(10),activite varchar(10),key INTEGER PRIMARY KEY AUTOINCREMENT)"));
        query.clear();
        query.finish();
        badge_db.close();
        model.clear();
        model.deleteLater();
    }

    badge_db_etat=-1;
    supervision_badge_db_status(1); //pas de logo database
    http_etat=0;
    time_out=1;
    http_erreurs=0;

    supervision_http_status(-2); //affichage du logo alerte_http
    priv_attente_ip(0); // param ok=0 recherche adresse ip (pas de lancement de /etc/init.d/networking restart déja fait au reset)
    supervision_http_status(-1); //affichage du logo alerte_http

    // Lecture database config supervision
    http_server.clear();
    http_port_https.clear();http_port_http.clear();http_service.clear();http_user.clear();http_password.clear();http_timeout='5';
    emit signal_process_init_http(); // lecture datatabase_setup.db pour affichage informations du serveur sur IHM
    supervision_init_http(); //Connexion serveur http/https (chronotime) et suppression logo alerte_http si pas d'erreur

}

/**
 * @brief Supervision_qobject::demarreTrigger
 * - à l'init demarrage du premier trigger aléatoirement entre 1 et 10 secondes.
 * - appelé par supervision_init_http()
 * - appel priv_trigger() (Stimer->start)
 * @param high
 * @param low
 */
void Supervision_qobject::demarreTrigger(int high=10, int low=1)
{
    // demarrage du premier trigger aléatoirement entre 1 et 10 secondes.
    if(high < low)
    {
        int i = low;
        low = high;
        high = i;
    }
    qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);
    int st = 1000 * (qrand() % ((high + 1) - low) + low);
    Stimer->start(st); //appel priv_trigger()
}

/**
 * @brief Supervision_qobject::supervision_init_http
 * - Connexion serveur http/https (chronotime)
 * - appel supervision_http_status() param -1 //affichage logo alerte
 * - appel attente_ssl_certificat() en cas de serveur https
 * - gestion time2bii 1.1.13 Serveur Chronotime (my.crypt) ou Gnet (my.cryptGnet)
 * - appel supervision_http_status() param 1 //suppression logo alerte
 * - appel demarreTrigger() premier trigger Stimer
 */
void Supervision_qobject::supervision_init_http(void)
{
    supervisionlog->debug("supervision init_http");
    timerloop.stop();
    http_initok=0;
    http_etat=0;
    time_out=1;
    http_erreurs=0;

    supervision_http_status(-1); //affichage du logo alerte_http

    QString a=QString("Configuration http: serveur= %1 port https= %2 port http= %3 service= %4 user= %5 password= %6 timeout= %7 ")
            .arg(http_server).arg(http_port_https).arg(http_port_http).arg(http_service).arg(http_user).arg(http_password).arg(http_timeout);
    supervisionlog->debug(a.toStdString());

    QString UrlString;
    if (!http_server.isEmpty())
    {
        qDebug() << "http_server not empty";
        if(!http_port_https.isEmpty()) //Préparer utilisation HTTPS SSL
        {
            qDebug() << "http_port_https not empty";
            ssl_certificat.setFileName("certificat.crt");
            ssl_certificat.remove();
            attente_ssl_certificat();

            QList<QSslCertificate> cert = QSslCertificate::fromPath(QLatin1String("certificat.crt"),QSsl::Pem,QRegExp::FixedString);
            ssl_certificat.remove();
            SslConfiguration= new QSslConfiguration(QSslConfiguration::defaultConfiguration());
            if (!cert.isEmpty())
            {
                qDebug() << "cert not empty";
                QSslKey mykey(cert.first().publicKey());
                SslConfiguration->setCaCertificates(cert);
                SslConfiguration->setLocalCertificate(cert.first());
                SslConfiguration->setPrivateKey(mykey);
                UrlString=QString("https://%1:%2%3").arg(http_server).arg(http_port_https).arg(http_service);
            }
            else
            {
                qDebug() << "cert empty";
                http_port_https.clear();
                UrlString=QString("http://%1:%2%3").arg(http_server).arg(http_port_http).arg(http_service);
            }
        }
        else
        {
            qDebug() << "http_port_https empty";
            UrlString=QString("http://%1:%2%3").arg(http_server).arg(http_port_http).arg(http_service);
        }
        url.setUrl(UrlString);
        request->setUrl(url);
        request->setHeader(QNetworkRequest::ContentTypeHeader,"text/plain");
        request->setUrl(url);
        // Config pour HTTPS SSL
        if (!http_port_https.isEmpty()) request->setSslConfiguration(*SslConfiguration);
    }
    version_logiciel.clear();
    version_logiciel.append(version_logiciel_i);
    // MODIF version time2bii-1.13
    //Fichier de cryptage different si Serveur Chronotime (my.crypt) ou Gnet (my.cryptGnet)
    QFile in;
    if (http_service.contains("chronotime",Qt::CaseInsensitive)) in.setFileName("/var/x7/contents/ssl/my.crypt");
    else in.setFileName("/var/x7/contents/ssl/my.cryptGnet");
    if (in.open( QFile::ReadOnly))
    {
        QTextStream stream_in(&in);
        key_crypt = stream_in.readAll();
        if (key_crypt.isEmpty())
        {
            key_crypt.append("on peut rire de tout mais pas avec tout le monde");
            version_logiciel.append(" (b)");
        }
        else
        {
            Encryption *encry = new Encryption();
            string msg= encry->encryption("on peut rire de tout mais pas avec tout le monde", QString(key_crypt), "AES_base64", false);
            key_crypt= QString::fromStdString(msg);
            version_logiciel.append(" (c)");
        }
        in.close();
    }
    else
    {
        key_crypt.clear();
        version_logiciel.append(" (a)");
    }
    supervisionlog->debug("Cryptage: %s", key_crypt.toStdString());
    Hashes *hash = new Hashes();
    string msg=hash->Sha256Function(key_crypt);
    //supervisionlog->debug("Cryptage HEX: "+QString::fromStdString(msg));
    supervisionlog->debug("Cryptage HEX: "+msg);
    supervisionlog->debug("Version Logiciel: "+version_logiciel.toStdString());
    //supervision_http_status(1); //suppression du logo alerte_http
    http_initok=1;
    demarreTrigger(30); //appel priv_trigger
}

//C'est quoi cette methode ? Elle marche vraiment ?
/**
 * @brief Supervision_qobject::attente_ssl_certificat
 * - Gestion des serveur https
 */
void Supervision_qobject::attente_ssl_certificat(void)
{
    supervisionlog->debug("attente_ssl_certificat");
    QString tmp;
/*    if (!http_port_https.isEmpty())
    {
  */  int i=0;
    while(!ssl_certificat.size())
        {
         QProcess *qProc= new QProcess(this);
         QString b= QString("openssl s_client -showcerts -connect %1:%2 ").arg(http_server).arg(http_port_https).toLatin1();
         supervisionlog->debug("openssl : " + b.toStdString());
         qProc->start(b);
         qProc->waitForReadyRead(2000);
         tmp = qProc->readAll();

         ssl_certificat.open(QIODevice::WriteOnly | QIODevice::Text);
         ssl_certificat.write(tmp.toLatin1());
         ssl_certificat.close();
         qProc->close();
         qProc->deleteLater();
         supervisionlog->debug("attente de certificat:"+tmp.toStdString());

         if (tmp.isEmpty())
         {
             i++;
             if (i >3)
             {
                 http_port_https.clear();
                 return;
             }
         }
        // QTest::qWait(1000);  // event processing
     }
}


//PRIVATE SLOTS
/**
 * @brief Supervision_qobject::priv_restart_reseau
 * - redemarrage reseau wlan
 * - appel /etc/init.d/networking restart
 */
void Supervision_qobject::priv_restart_reseau(void)
{
    supervisionlog->debug("priv_restart_reseau");
    return; //FIXME
    QString a;
    http_erreurs=0;
    QProcess *qProc = new QProcess(this);
    a= QString("ip route del");
    qProc->start(a);
    qProc->waitForReadyRead();
    qProc->readAll();
    qProc->close();
    qProc->deleteLater();

    supervisionlog->debug("redemarrage reseau wlan") ;
    QProcess *qProc1 = new QProcess(this);
    a= QString("/etc/init.d/networking restart");
    qProc1->start(a);
    QTest::qWait(1000);
    while ( qProc1->state()==QProcess::Running) QTest::qWait(100);
    supervisionlog->debug("Restart termine");
    qProc1->close();
    qProc1->deleteLater();
}

/**
 * @brief Supervision_qobject::priv_supervision_test_interface
 * - recherche les interfaces réseaux disponibles
 * @return
 */
QString Supervision_qobject::priv_supervision_test_interface()
{
    supervisionlog->debug("priv_supervision_test_interface");
    // test interface OK
    QList<QNetworkInterface> interfaces ;
    QString interface_name;
    interface_name.clear();
    interfaces = QNetworkInterface::allInterfaces();
    foreach(const QNetworkInterface & interface, interfaces)
    {
        if (interface.name().contains("eth") || interface.name().contains("wlan") || interface.name().contains("ppp"))
        {
            if (interface.flags() & QNetworkInterface::IsUp)
            {
                if (interface.flags()  & QNetworkInterface::IsRunning)
                {
                    interface_name=interface.name();
                }
            }
        }
    }

    if (interface_name.contains("wlan") || interface_name.contains("ppp"))
    {
        interfaces = QNetworkInterface::allInterfaces();
        foreach(const QNetworkInterface & interface, interfaces)
        {
            if (interface.name().contains("eth"))
            {
                if (interface.flags() & QNetworkInterface::IsUp)
                {
                    QProcess *qProc = new QProcess(this);
                    QString a= QString("ifconfig eth0 down");
                    qProc->start(a);
                    qProc->waitForReadyRead();
                    qProc->readAll();
                    qProc->close();
                    qProc->deleteLater();
                }
            }
        }
    }
    interfaces.clear();
    return(interface_name);
}

/**
 * @brief Supervision_qobject::priv_supervision_test_ip
 * - recherche adresse ip
 * - appel priv_supervision_test_interface(), recherche interfaces réseaux disponibles
 * - appel signal_x7GestConfig_init_mode_network(), envoie infos réseaux à l'IHM
 */
void Supervision_qobject::priv_supervision_test_ip()
{
    // test adresse ip OK
    supervisionlog->debug("priv_supervision_test_ip");
    QList<QNetworkInterface> interfaces ;
    QHostAddress val;

    QString interface_name=priv_supervision_test_interface();

    if (!interface_name.isNull())
    {
        interfaces = QNetworkInterface::allInterfaces();
        foreach(const QNetworkInterface & interface, interfaces)
        {
            if (interface.name()==interface_name)
            {
                QList<QNetworkAddressEntry> entries = interface.addressEntries();
                if (!entries.isEmpty())
                {
                    QNetworkAddressEntry entry = entries.first();
                    val = entry.ip();
                }
            }
        }
    }
    //envoi info reseau à l'IHM
    if (val != ip) {ip=val; signal_x7GestConfig_init_mode_network();}
    supervisionlog->debug("Interface name: "+ interface_name.toStdString() +" Ip: "+ ip.toString().toStdString());
    interfaces.clear();
}

/**
 * @brief Supervision_qobject::priv_attente_ip
 * - recherche adresse ip
 * - appel priv_restart_reseau() redémarrage reseau wlan
 * - appel priv_supervision_test_ip() recherche adresse ip
 * @param ok: 0-recherche adresse ip, 1-redemarrage interface réseau puis recherche ip
 */
void Supervision_qobject::priv_attente_ip(int ok)
{
    int b=0;
    if(ok) priv_restart_reseau(); //redemarrage interface réseau
    priv_supervision_test_ip(); //Met à jour variable ip

    while (ip.isNull())
    {
        if (b++ >10)//on redémarre le reseau pour la bieme fois
        {
            supervisionlog->debug("priv_attente_ip: b++ >10");
            b=0;
            priv_restart_reseau(); //redemarrage interface réseau
        }
        supervision_http_status(-1); //affichage du logo alerte_http
        QTest::qWait(1000);  // event processing
        priv_supervision_test_ip(); //Met à jour variable ip
    }
}

/**
 * @brief Supervision_qobject::nextIdPost
 * @return
 */
quint32 Supervision_qobject::nextIdPost()
{
    idPost++;
    return idPost;
}


/**
 * @brief Supervision_qobject::finishedSlot
 * - appelé par priv_trigger() avec le manager post request
 * - appel supervision_http_status() pour affichage logo alerte si pas d'erreur
 * - si il y avait des badgeage en cache on les supprime de database_setup.db
 * - appel au priv_trigger() Stimer->start pour un nouveau trigger dans 90secs
 * @param reply
 */
void Supervision_qobject::finishedSlot(QNetworkReply* reply)
{
    // no error received?
    supervisionlog->debug("finishedSlot");
    bool erreur = true;
    //demarrage du trigger dans 90 secondes
    int tt = 90*1000;

    // Test si la réponse c'est bien déroulé.
    if (reply->error() == QNetworkReply::NoError)
    {

        supervisionlog->debug("Test réseau, QNetworkReply::NoError");
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        supervisionlog->debug("Test réseau, QNetworkRequest reply : %d", statusCodeV.toInt());
        if (statusCodeV.toInt() >=200 && statusCodeV.toInt() < 300 )
        {            
            QString rst(reply->readAll());
            if (rst.contains("OK"))
            {
                erreur = false;
            }
        }
    }
    supervisionlog->debug("Test réseau, erreur : %d", erreur);
    if(erreur)
    {
        supervision_http_status(-1); //affichage du logo alerte_http
        http_erreurs++;
    }
    else
    {
        http_erreurs=0;
        supervision_http_status(1); //suppression du logo alerte_http
    }

    // cas d'un badgeage.
    if (reply->property("idBadgeage").isValid())
    {
        supervisionlog->debug("c'est un badgeage");
        int id = reply->property("idBadgeage").toInt();
        if(badgeCache.contains(id))
        {
            badgeage *b = badgeCache[id];
            if(b->dbKey != 0)
            {
                supervisionlog->debug("badge en cache");
                //le badge est en base de donnée.
                if(badge_db.open())
                {
                    int nb = 0;
                    QSqlQuery query(QString("SELECT count(*) from badge where key=%1").arg(b->dbKey),badge_db);
                    if(query.next())
                    {
                        supervisionlog->debug("badge sauvegardé");
                        nb = query.value(0).toInt();
                    }

                    if(erreur == false)
                    {
                        if(nb!=0)
                        {
                            supervisionlog->debug("on efface le badge dans la bdd ");
                            query.exec(QString("delete from badge where key=%1").arg(b->dbKey));
                            if (query.lastError().type() != 0)
                                supervision_badge_db_status(0); //affichage logo database
                            else
                            {
                                //on continue a envoyer les badges sauvegardés toute les 10 ms.
                                tt = 10;
                                supervision_badge_db_status(1); //pas de logo database
                            }
                        }
                    }
                    else
                    {
                        supervisionlog->debug("on n'efface pas y a un problème");
                        //on ne supprime pas le badgeage car il y a une erreur.
                    }
                    badge_db.close();
                }
            }else
            {
                //badge n'est pas en base
                supervisionlog->debug("finishedSlot badge n'est pas en base");
                if(erreur == true)
                {
                    //il y a eu une erreur dans la réponse, on sauvegarde le badge.
                    supervisionlog->debug("ya un bug. on sauvegarde le badgeage");
                    sauvegardeBadge(id);
                }else
                {
                    if(badge_db.open())
                    {
                        int nb = 0;
                        QSqlQuery query(QString("SELECT count(*) from badge"),badge_db);
                        if(query.next())
                        {
                            supervisionlog->debug("badge sauvegardé");
                            nb = query.value(0).toInt();
                            if(nb!=0)
                            {
                                tt=10;
                            }
                        }
                        badge_db.close();
                    }
                    //le badgeage a bien été envoyé on ne le sauvegarde pas.
                    supervisionlog->debug("le badgeage est ok");
                }


            }
        }else
        {
            //Une réponse d'un badge que je ne connais pas ? un doublon ?
            supervisionlog->debug("c'est quoi ce bins");
        }
    }else //pas de badgeage en cache
    {
        //ce n'est pas un badgeage. un trigger ?
        supervisionlog->debug("a voilà le trigger");
    }

    //demarrage du trigger dans tt secondes
    Stimer->start(tt);
    reply->deleteLater();
}

/**
 * @brief Supervision_qobject::priv_trigger
 * - appelé par demarreTrigger() avec Stimer à l'init
 * - appelé par finishedSlot() en récurrence
 * - verif interface reseau ok
 * - lecture badge_db /var/x7/contents/database_badge.db
 * - si il y a des badgeage en cache on les envoie au serveur par le manager post request
 */
void Supervision_qobject::priv_trigger(void)
{
    supervisionlog->debug("priv_trigger");
    if(http_erreurs<=5)
        priv_attente_ip(0);  //0-recherche adresse ip,
    else
        priv_attente_ip(1);  //1-redemarrage interface réseau puis recherche ip

    if(!badge_db.open())
    {
        supervision_badge_db_status(0); //affichage logo database
        supervisionlog->debug("pas de connection database");
    }
    else
    {
        QSqlQuery query(badge_db);
        query.prepare("SELECT matric, heure, jour, activite, key FROM badge LIMIT 1");
        query.exec();
        if(query.next())
        {
            supervisionlog->debug("badgeage sauvegarde");
            int key=query.record().value("key").toInt();
            QString activite = query.record().value("activite").toString();
            QString csn = query.record().value("matric").toString();

//            QTime t = QTime::fromString(query.record().value("heure").toString(), "hh:mm:ss");
//            QDate d = QDate::fromString(query.record().value("jour").toString(), "yyyy-MM-dd");

            QDateTime dt = QDateTime::fromString(query.record().value("jour").toString()+" "+query.record().value("heure").toString(), "yyyy-MM-dd hh:mm:ss");
            badgeage *b = new badgeage(csn,dt,activite,key);

            quint32 id = nextIdPost();
            badgeCache.insert(id,b);

            QString a=QString::number(QDateTime::currentMSecsSinceEpoch());
            a.append(" "+b->toString()+" "+time2bii_name+" "+ ip.toString()+" "+"M");
            supervisionlog->debug(" trigger: " + a.toStdString());
            QByteArray s1=encode_string(a.toLatin1());
            s1.insert(0,"id=");
            QNetworkReply *reply =manager->post(*request,s1);
            reply->setProperty("idBadgeage", id);
        }
        else
        {
            supervisionlog->debug("pas de badgeage : trigger");
            QString a("Trigger "+time2bii_name+" "+ ip.toString()+" ");
            a.append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
            QByteArray s1=encode_string(a.toLatin1());
            s1.insert(0,"id=");
            manager->post(*request,s1);
        }
        badge_db.close();
    }
}

/**
 * @brief Supervision_qobject::provideAuthentication
 * @param reply
 * @param ator
 */
void Supervision_qobject::provideAuthentication(QNetworkReply *reply, QAuthenticator *ator)
{
    supervisionlog->debug("authentication");
    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    ator->setUser(http_user);
    ator->setPassword(http_password);
}

//
// PUBLIC SLOTS
//

/**
 * @brief Supervision_qobject::ihm_post_badge
 * - appelé par signal_supervision_ihm_post_badge quand un badgeage survient (X7LecteurRF::recoitDataDuPort())
 * - appel sauvegardeBadge()
 * @param badge
 * @param j
 * @param h
 * @param activite
 */
void Supervision_qobject::ihm_post_badge(QString badge,QString j,QString h,QString activite)
{
    supervisionlog->debug("ihm_post_badge");
    QTime t = QTime::fromString(h, Qt::TextDate);
    QDate d = QDate::fromString(j, Qt::ISODate);
    QDateTime dt(d,t);
    supervisionlog->debug("badgeage : %s %s %s %s", badge, j, h, activite);
    badgeage *b = new badgeage(badge,dt,activite,0);

    quint32 id = nextIdPost();
    badgeCache.insert(id,b);

    if (!http_server.isEmpty() && http_initok==1 && !(ip.isNull()) )
    {
        QString s=QString::number(QDateTime::currentMSecsSinceEpoch());
        s.append(" "+b->toString()+" "+time2bii_name+" "+ ip.toString()+" "+"T");

        QByteArray s1=encode_string(s.toLatin1());
        s1.insert(0,"id=");
        supervisionlog->debug("badgeage : "+s1);

        QNetworkReply *reply =manager->post(*request,s1);
        reply->setProperty("idBadgeage", id);
    }
    else
    {
        sauvegardeBadge(id);
    }
}

/**
 * @brief Supervision_qobject::sauvegardeBadge
 * - sauvegarde badgeage en bdd
 * @param id
 */
void Supervision_qobject::sauvegardeBadge(int id)
{
    supervisionlog->debug("sauvegardeBadge");
    if (!badge_db.open())
    {
        supervision_badge_db_status(0); //affichage logo database
        supervisionlog->debug("database not open");
    }
    else if(badgeCache.contains(id))
    {
        badgeage *b = badgeCache[id];

        QSqlQuery query(badge_db);
        query.prepare("INSERT INTO badge (heure, jour, matric,activite)" "VALUES (:heure, :jour, :matric, :activite);");
        query.bindValue(":heure",b->timeToString());
        query.bindValue(":jour", b->dateToString());
        query.bindValue(":matric", b->csn);
        query.bindValue(":activite",b->activite);
        query.exec();

        if (query.lastError().type() != 0)
        {
            supervision_badge_db_status(0); //affichage logo database
            b->dbKey =  query.lastInsertId().toInt();
        }
        else
        {
            supervision_badge_db_status(1); //pas de logo database
        }
        //query.finish();

        //badge_db.commit();
        badge_db.close();
    }
}


/*-------------- Codage chaine totale badgeage  -----------*/
/**
 * @brief Supervision_qobject::encode_string
 * @param s
 * @return
 */
QByteArray Supervision_qobject::encode_string(QByteArray s)
{
    supervisionlog->debug("encode_string");

  if (key_crypt.isEmpty())  return(s.toBase64());
  else
    {
    using namespace CryptoPP;
    Encryption *encry = new Encryption();
    //FIXME fuite mémoire ici
    string msg= encry->encryption(key_crypt, QString(s), "AES_base64", true);
    QString b= QString::fromStdString(msg);
    return(b.toLatin1());
    }
}

/**
 * @brief Supervision_qobject::getTime2bii_name
 * @return
 */
QString Supervision_qobject::getTime2bii_name() const
{
    supervisionlog->debug("getTime2bii_name");
    return time2bii_name;
}

/**
 * @brief Supervision_qobject::setTime2bii_name
 * @param value
 */
void Supervision_qobject::setTime2bii_name(const QString &value)
{
    supervisionlog->debug("setTime2bii_name");
    time2bii_name = value;
}

/**
 * @brief Supervision_qobject::supervision_http_status
 * - Check status connexion http pour affichage logo alerte
 * - appel signal ihm_http_status pour maj ihm
 *  @param a<=0 le logo est affiché (qml opacity=1), a=1 le logo est supprimé, connexion ok (qml opacity=0)
 */
void Supervision_qobject::supervision_http_status(int a)
{
    supervisionlog->debug("supervision http_status");
    // a l'init http_etat=0
    if (http_etat!=a)
    {
        supervisionlog->debug("http_etat %d != a %d ", http_etat, a);
        http_etat=a;
        if (a <=0 )
        {
            a=0;
            supervisionlog->debug("signal_ihm_http_status logo alerte affiché");
        }
        else //a=1
        {
            supervisionlog->debug("signal_ihm_http_status logo alerte supprimé, connexion ok");
        }
        emit signal_ihm_http_status(QVariant(a));// signal pour afficher le logo alerte_http sur l'ihm page d'accueil
    }

 }

/**
 * @brief Supervision_qobject::supervision_badge_db_status
 * - appel signal_ihm_badge_db_status() pour afficher en IHM le logo badgeage
 * - à l'init a=1, pas d'affichage du logo
 * - si a=0 , on affiche le logo badgeage
 * @param a
 */
void Supervision_qobject::supervision_badge_db_status(int a)
{
    supervisionlog->debug("supervision_badge_db_status");
     if (badge_db_etat!=a)
        {
         supervisionlog->debug("badge_db_etat %d != a %d ", badge_db_etat, a);
         if (a==0)
             supervisionlog->debug("logo database affiché");
         else //a=0
         {
             supervisionlog->debug("pas de logo database");
         }
         emit signal_ihm_badge_db_status(QVariant(a));// signal vers ihm
         badge_db_etat=a;
        }
 }

/**
 * @brief (NOT USED) Supervision_qobject::customEvent
 * @param event
 */
void Supervision_qobject::customEvent ( QEvent * event )
{
    if(T2bEvent::custom_type()== event->type())
    {
        T2bEvent *e = (T2bEvent *)event;

        switch (e->eventType())
        {

            case EventType::BADGE_UPDATE_NOM:
                break;
            default:
                break;

        }
    }
}


// QTHREAD
/**
 * @brief Supervision_qthread::init
 * - appel setTerminationEnabled() pour utilisation de QThread::terminate()
 */
void Supervision_qthread::supervision_qthread_init(void)
{
    setTerminationEnabled(true);
}

/**
 * @brief Supervision_qthread::run
 * - appel Supervision_qobject::supervision_init()
 */
void Supervision_qthread::run(void)
{
    supervision_qthread_init();
    supervision.supervision_init();
    exec();
}



