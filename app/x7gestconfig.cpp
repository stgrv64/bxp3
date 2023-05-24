/**
 * @file
 * @brief X7GestConfig class definition
 * *******************************************/
#include "x7gestconfig.h"

#include <QDir>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QTimer>
#include <QHostInfo>

#include "x7customevent.h"
#include "x7gestlecteur.h"
#include "x7portserie.h"
#include "x7carteio.h"
#include <supervision.h>

//Nom des fichiers de BDD sans extension .db
QString X7GestConfig::bddSetupName = "database_setup";
QString X7GestConfig::bddBadgeName = "database_badge";

X7GestConfig * X7GestConfig::instance = 0;

log4cpp::Category * X7GestConfig::log4cpp = &log4cpp::Category::getInstance("X7GestConfig");

/**
 * @brief X7GestConfig::X7GestConfig
 * @param parent
 */
X7GestConfig::X7GestConfig(QObject *parent) : QObject(parent)
{
    log4cpp->debug("constructeur");
    //Demarrage automatique du gestionnaire après sont instanciation.
    QTimer::singleShot(10, this, SLOT(startX7GestConfig()));
    _portSerieLecteur = nullptr;
    _audioVolume=1;

}
/**
 * @brief X7GestConfig::getInstance
 * @return
 */
X7GestConfig *X7GestConfig::getInstance()
{
    log4cpp->debug("getInstance");
    if ( !X7GestConfig::instance )
    {
        X7GestConfig * gestConfig = new X7GestConfig();
        X7GestConfig::instance = gestConfig;
    }

    return X7GestConfig::instance;

}
/**
 * @brief X7GestConfig::startX7GestConfig
 * - appelé à l'init de l'objet
 * - appel updateDateTimeNtp()
 * - creation d'un Qthread serialThread
 */
void X7GestConfig::startX7GestConfig()
{
    log4cpp->debug("startX7GestConfig");
    QSqlDatabase::addDatabase("QSQLITE",bddSetupName);
    QSqlDatabase::addDatabase("QSQLITE",bddBadgeName);

    //setIpV4Config(QString("dhcp"),QString(),QString(),QString());


    //FIXME gérer l'activation du serveur NTP au lieu de la cadence ?
    timer_ntp =new QTimer();
    timer_ntp->setSingleShot(true);
    connect(timer_ntp,SIGNAL(timeout()),this,SLOT(updateDateTimeNtp()));
    //timer_ntp->start(60000);

    // Instanciation du port serie 1 par defaut
    _portSerieLecteur = new X7PortSerie();
    _portSerieLecteur->config(X7PortSerie::X7_READER_1);

    // Déplacement du port serie sur un nouveau thread.
    QThread *serialThread = new QThread;
    _portSerieLecteur->moveToThread(serialThread);

    serialThread->start();

    QObject::connect(serialThread, SIGNAL(finished()), _portSerieLecteur, SLOT(close()));
    QObject::connect(qApp, SIGNAL(aboutToQuit()), serialThread, SLOT(quit()));

    // Instanciation du port serie pour la config de la x7
    _portSerieServiceDevice = new X7PortSerie();
    _portSerieServiceDevice->config(X7PortSerie::X7_INTERNAL_IO);

    ouvrirBdd(bddSetupName);
    //Lecteur de la Configuration
    readConfig();
    initHostName();


    //Initialisation du lecteur correspondant a la config.
    X7GestLecteur::getInstance()->initLecteur();    
    X7CarteIO::getInstance()->initCarteIOPort();

    // init du retro eclairage
    //x7GestConfig_init_divers();//doublon?
}

/**
 * @brief X7GestConfig::quitter
 * - appelé dans (NOT USED) X7GestConfig::tsCalibrate
 */
void X7GestConfig::quitter()
{
    QTimer::singleShot( 0, qApp, SLOT(quit()));
}

/**
 * @brief X7GestConfig::getPortSerieLecteur
 * - appelé dans X7GestLecteur::initLecteur()
 * @return
 */
X7PortSerie *X7GestConfig::getPortSerieLecteur() const
{
    log4cpp->debug("getPortSerieLecteur");
    return _portSerieLecteur;
}

/**
 * @brief X7GestConfig::getPortSerieServiceDevice
 * - appelé dans X7CarteIO::initCarteIOPort()
 * @return
 */
X7PortSerie *X7GestConfig::getPortSerieServiceDevice() const
{
    log4cpp->debug("getPortSerieServiceDevice");
    return _portSerieServiceDevice;
}
/**
 * @brief (NOT USED) X7GestConfig::initTsCalibrate
 */
void X7GestConfig::initTsCalibrate()
{
    // Rien a faire pour le X7 ?
}
/**
 * @brief (NOT USED) X7GestConfig::tsCalibrate
 */
void X7GestConfig::tsCalibrate()
{
    log4cpp->debug("tsCalibrate");
    QString a= QString("./ts_calibrate_shell");
    QProcess *qProc = new QProcess(this);
    qProc->startDetached(a);
    quitter();
}
/**
 * @brief X7GestConfig::getIpLocal
 * - appelé dans X7GestConfig::updateDateTimeNtp()
 * @return
 */
quint32 X7GestConfig::getIpLocal()
{
    log4cpp->debug("getIpLocal");
    readConfig();
    quint32 ip;
    ip=0;

    // Gestion d'une seule interface reseau Ipv4.
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    foreach(QHostAddress hostAddress,QNetworkInterface::allAddresses())
    {
        if(hostAddress != localhost && hostAddress.protocol() == QAbstractSocket::IPv4Protocol)
        {
            ip = hostAddress.toIPv4Address();
            log4cpp->debug( "Adresse IP X7 :" + hostAddress.toString().toStdString());
            break;
        }
    }

    if(ip==0)
    {
        qWarning() << "Ipv4 non initialisee";
        //QProcess::startDetached("dhcpcd", QStringList() << "-n");
    }
    return ip;
}
/**
 * @brief X7GestConfig::dhcp
 * - utilisé dans FormController::service() et NomController::readXml_nom()
 * @return
 */
bool X7GestConfig::dhcp()
{
    QString eth_id = getConnmanWiredService();

    log4cpp->info(QString("Dhcp Service Ethernet : %1").arg(eth_id).toStdString());

    QFileInfo cf(QString("/var/lib/connman/%1/settings").arg(eth_id));

    if(cf.exists())
    {
        QSettings config(cf.absoluteFilePath(),QSettings::IniFormat);
        config.beginGroup(eth_id);
        QString ipm = config.value("IPv4.method").toString();
        log4cpp->info(QString("Dhcp IPv4.method : %1").arg(ipm).toStdString());

        if(ipm.compare("dhcp")==0)
            return true;
    } else
    {
        log4cpp->info(QString("Dhcp le fichier n'existe pas : %1").arg(cf.absoluteFilePath()).toStdString());
    }
    return false;
}



/**
 * @brief X7GestConfig::initHostName
 * - appelé à l'init startX7GestConfig()
 */
void X7GestConfig::initHostName()
{
    log4cpp->debug("initHostName");
    QString lhn = QHostInfo::localHostName();

    log4cpp->info(QString("initHostName : %1").arg(lhn).toStdString());
    //FIXME ne pas utilise de global
    supervision_thread.supervision.time2bii_name=lhn.simplified();

//    setHostName("azerty");
//    setHostName("123");
//    setHostName("&zesd");
//    setHostName("az&zesd");
//    setHostName("az zesd");
//    setHostName("az-ze4");
//    setHostName("x7");
//    setHostName("Time2bii X7");
//    setHostName("Météo");
//    setHostName("x7.inetum.com");
//    setHostName("x7..inetum.com");
//    setHostName(".x7.inetum.com");
//    setHostName("x7?inetum.com");

    //code repris de la time2bii
//    QProcess *qProc3 = new QProcess(this);
//    qProc3->start("hostname");
//    qProc3->waitForReadyRead();
//    QString time2bii_name=QString::fromLatin1(qProc3->readAll()).simplified();
//    log4cpp->info(QString("hostname : %1").arg(time2bii_name).toStdString());
//    //FIXME ne pas utilise de global
//    supervision_thread.supervision.time2bii_name=time2bii_name.simplified();
//    qProc3->close();
//    qProc3->deleteLater();

}
/**
 * @brief X7GestConfig::setHostName
 * - appelé dans x7GestConfig_set_mode_network()
 * - utilisé à l'init de NomController::NomController()
 * @param nom
 */
void X7GestConfig::setHostName(QString nom)
{      
    log4cpp->debug("setHostName");
    //regexp pour les noms de machine RFC 1178
    //http://www.faqs.org/rfcs/rfc1178.html
    QRegExp rx("^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\\-]*[a-zA-Z0-9])\\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\\-]*[A-Za-z0-9])$");

    if(rx.exactMatch(nom))
        log4cpp->info(QString("setHostName %1 : OK").arg(nom).toStdString());
    else
    {
        log4cpp->warn(QString("setHostName %1 : KO").arg(nom).toStdString());
        return;
    }

    QString lhn = QHostInfo::localHostName();

    //pas de maj si non identique
    if (lhn != nom)
    {
        //on enregistre le hostname
        QFile out("/etc/hostname");
        if (out.open( QFile::WriteOnly))
        {
            QTextStream stream_out(&out);
            stream_out << nom << endl;
            stream_out.flush();
            out.close();
            supervision_thread.supervision.time2bii_name=nom;
        } else
        {
            log4cpp->warn("impossible d ouvrir /etc/hostname");
            return;
        }

        //Execution du programme hostname pour modification
        QProcess *qProc = new QProcess(this);
        qProc->start("hostname -F /etc/hostname");
        if(!qProc->waitForFinished())
        {
            log4cpp->warn(QString(qProc->readAllStandardError()).toStdString());
            log4cpp->warn(QString(qProc->readAllStandardOutput()).toStdString());
            log4cpp->warn("erreur du programme hostname -F /etc/hostname");
        }
        qProc->deleteLater();

        // --- verification ---
        //TODO La vérification est en trop je pense.
//        QProcess *qProc3 = new QProcess(this);
//        qProc3->start("hostname");
//        qProc3->waitForReadyRead();
//        QString time2bii_name = QString::fromLatin1(qProc3->readAll()).simplified();
//        supervision_thread.supervision.time2bii_name=time2bii_name;
//        qProc2->waitForFinished();
//        qProc3->deleteLater();
    } else
        log4cpp->info(QString("setHostName identique : %1").arg(lhn).toStdString());

}

/**
 * @brief X7GestConfig::getConnmanWiredService
 * utilisé dans dhcp() et setIpV4Config()
 * @return
 */
QString X7GestConfig::getConnmanWiredService()
{
    log4cpp->debug("getConnmanWiredService");
    QProcess *qProc = new QProcess(this);

    QString eth_id;

    qProc->start("/bin/sh", { "-c" , "connmanctl services | grep Wired | awk -F' ' '{ print $3 }'" });
    //FIXME ici bloquage loopevent pour 30 secondes max
    if(qProc->waitForReadyRead())
    {
        eth_id = QString(qProc->readAll()).simplified();

    } else
    {
        log4cpp->warn(QString(qProc->readAllStandardError()).toStdString());
        log4cpp->warn(QString(qProc->readAllStandardOutput()).toStdString());
        log4cpp->warn("Pas d'eth0 trouvé");
    }

    qProc->waitForFinished(1000);

    if(qProc->state() != QProcess::NotRunning)
        qProc->kill();

    qProc->deleteLater();


    return eth_id;
}

/**
 * @brief X7GestConfig::setIpV4Config
 * - methode pour activer le réseau DHCP ou statique IP
 * - appelé à l'init de NomController::NomController()
 * @param mode
 * @param config
 */
void X7GestConfig::setIpV4Config(modeIpV4 mode, configIpV4 config)
{
    log4cpp->debug("setIpV4Config");

    if (mode == modeIpV4::FIX && (config.ip.isEmpty() || config.netmask.isEmpty()))
        return;

    QProcess *qProc = new QProcess(this);

    QString eth_id = getConnmanWiredService();


    log4cpp->info(QString("Service Ethernet : %1").arg(eth_id).toStdString());


    if(mode == modeIpV4::DHCP)
    {
        QString cmd = QString("connmanctl config %1 --ipv4 dhcp").arg(eth_id);
        log4cpp->info(QString("cmd dhcp : %1").arg(cmd).toStdString());

        qProc->start(cmd);
        //FIXME ici bloquage loopevent pour 30 secondes max
        if(!qProc->waitForFinished())
        {
            log4cpp->warn(QString(qProc->readAllStandardError()).toStdString());
            log4cpp->warn(QString(qProc->readAllStandardOutput()).toStdString());
            log4cpp->warn("la configuration ipv4 dhcp a échoué");
        }

    }
    else // en statique
    {        
        QString cmd = QString("connmanctl config %1 --ipv4 manual %2 %3 %4").arg(eth_id,config.ip,config.netmask,config.gateway);
        if(!config.dns.isEmpty())
        {
            cmd.append(QString(" --nameservers"));
            foreach (QString dns, config.dns)
            {
                cmd.append(QString(" %1").arg(dns));
            }
        }
        log4cpp->info(QString("cmd statique :%1").arg(cmd).toStdString());

        qProc->start(cmd);
        //FIXME ici bloquage loopevent pour 30 secondes max
        if(!qProc->waitForFinished())
        {
            log4cpp->warn(QString(qProc->readAllStandardError()).toStdString());
            log4cpp->warn(QString(qProc->readAllStandardOutput()).toStdString());
            log4cpp->warn("la configuration ipv4 statique a échoué");
        }
    }

    if(qProc->state() != QProcess::NotRunning)
        qProc->kill();

    qProc->deleteLater();
}


/**
 * @brief X7GestConfig::x7GestConfig_set_mode_network
 * - methode pour activer le réseau DHCP ou statique IP
 * - appel setIpV4Config()
 * - appel x7GestConfig_init_mode_network() à l'init
 * @param mode
 * @param nom
 * @param ip
 * @param netmask
 * @param gateway
 * @param dns1
 * @param dns2
 */
void X7GestConfig::x7GestConfig_set_mode_network(int mode,QString nom,QString ip,QString netmask,QString gateway,QString dns1,QString dns2)
{

    configIpV4 config;

    config.ip = ip;
    config.netmask = netmask;
    config.gateway = gateway;
    config.dns << dns1 << dns2 ;

    if(mode==0)
    {
        //0 DHCP
        setHostName(nom);


        setIpV4Config(modeIpV4::DHCP,config);

    }else if(mode==1)
    {
        //1 IPFIX
        setHostName(nom);
        setIpV4Config(modeIpV4::FIX,config);

        //FIXME lire les dns dans connman
        if (!dns1.isEmpty() || !dns2.isEmpty())
        {
            QFile outdns("/etc/resolv.conf");
            QTextStream stream_out(&outdns);
            QString dns;
            if (!dns1.isEmpty()) dns.append("nameserver "+dns1+'\n');
            if (!dns2.isEmpty()) dns.append("nameserver "+dns2+'\n');
            outdns.open( QFile::WriteOnly);
            stream_out << dns;
            stream_out.flush();
            outdns.close();
        }


    }else
    {
        //INIT ?
        x7GestConfig_init_mode_network();
    }

}

//FIXME voir l'autre methode en commentaire "init_mode_network"
/**
 * @brief X7GestConfig::x7GestConfig_init_mode_network
 * - récupère informations réseau (dns ...) pour affichage IHM
 * - appel signal_ihm_init_mode_network()
 */
void X7GestConfig::x7GestConfig_init_mode_network(void)
{
    log4cpp->debug("init_mode_network");
    QHostAddress ip,netmask,gateway;
    QString dns1,dns2;

    QString a= QString("ip route");
    QProcess *qProc = new QProcess(this);
    qProc->start(a);
    qProc->waitForReadyRead();
    QString tmp = qProc->readAll(); // tmp = "default via 172.16.66.1 dev eth0 <cr><lf>172.16.66.0/23 dev eth0 proto kernel scope link src 172.16.66.33"
    qProc->close();
    qProc->deleteLater();

    gateway=tmp.section(' ',2,2);

    QString nom=supervision_thread.supervision.time2bii_name;

    ip.clear();
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach(const QNetworkInterface & interface, interfaces)
    {
        if (interface.name().contains("eth") || interface.name().contains("wlan") || interface.name().contains("ppp"))
        {
            QList<QNetworkAddressEntry> entries = interface.addressEntries();
            if (!entries.isEmpty())
            {
                QNetworkAddressEntry entry = entries.first();
                ip = entry.ip();
                netmask =entry.netmask();
            }
        }
    }

    QFile in("/etc/resolv.conf");
    if (in.open( QFile::ReadOnly))
    {
        QTextStream stream_in(&in);
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

        in.close();
    }
    //appel mainView.qml ihm_init_mode_network
    signal_ihm_init_mode_network(nom,ip.toString(),netmask.toString(),gateway.toString(),dns1,dns2);
}

// TODO corriger cette methode et la réactiver
//void X7GestConfig::x7GestConfig_init_mode_network()
//{
//    QString nom = QHostInfo::localHostName();

//    QString eth_id = getConnmanWiredService();
//    QFileInfo cf(QString("/var/lib/connman/%1/settings").arg(eth_id));

//    quint8 mask;
//    QString ip, gateway, fullMask, dns1, dns2;
//    if(cf.exists())
//    {
//        QSettings config(cf.absoluteFilePath(),QSettings::IniFormat);
//        config.beginGroup(eth_id);
//        QString ipm = config.value("IPv4.method").toString();

//        if(ipm.compare("dhcp")==0)
//            ip = config.value("IPv4.DHCP.LastAddress").toString();
//        else
//            ip = config.value("IPv4.local_address").toString();
//        //FIXME Il faut lire directement dans connman et pas dans les fichiers pour le DHCP
//        gateway = config.value("IPv4.gateway").toString();
//        mask = config.value("IPv4.netmask_prefixlen").toInt();

//        fullMask = QString("1").repeated(mask) + QString("0").repeated(32-mask);
//        fullMask = binaryOctetsToDecimal(fullMask);


//        //gateway = config.value("IPv4.gateway").toString();
//        log4cpp->info(QString("INIT INFO IPv4 : %1 , %2, %3").arg(ip).arg(fullMask).arg(gateway).toStdString());
//    } else
//    {
//        log4cpp->info(QString("INIT INFO IPv4 le fichier n'existe pas : %1").arg(cf.absoluteFilePath()).toStdString());
//    }

//    signal_ihm_init_mode_network(nom,ip,fullMask,gateway,dns1,dns2);


//}

/**
 * @brief X7GestConfig::binaryOctetsToDecimal
 * @param IPbinary
 * @return
 */
QString X7GestConfig::binaryOctetsToDecimal(QString& IPbinary)
{
    QStringList netAddressList;
    for (int i = 0; i < 32; i+=8)
    {
        QString intOctet = QString::number(QStringRef(&IPbinary, i, 8).toString().toInt(nullptr, 2));
        netAddressList.append(intOctet);
    }

    return netAddressList.join(".");
}

/**
 * @brief X7GestConfig::setAudioVolume
 * - permet de changer le volume de l'application par action utilisateur
 * @param a si 0 muet si 1 incrément entre les valeurs 10-100%.
 */
void X7GestConfig::setAudioVolume(QString a)
{
    log4cpp->debug("audio_set_volume");

    QString vol;
    if (a.compare("1")==0)
    {

        switch (_audioVolume) {
        case 1:
            vol = "10%";
            break;
        case 2:
            vol = "30%";
            break;
        case 3:
            vol = "50%";
            break;
        case 4:
            vol = "70%";
            break;
        case 5:
            vol = "80%";
            break;
        case 6:
            vol = "90%";
            break;
        case 7:
            vol = "95%";
            break;
        default:
            break;
        }
    }
    else vol = "0%";

    if(_audioVolume<7)
        _audioVolume++;
    else
        _audioVolume=1;

    //script qui joue sur volume amixer
    QString b=QString("./audio_volume %1").arg(vol);
    log4cpp->debug(b.toStdString());
    QProcess *qProc = new QProcess(this);
    qProc->startDetached(b);
}

/**
 * @brief X7GestConfig::x7GestConfig_setWifi
 * - appel x7GestConfig_initWifi()
 * - appelé à l'init de NomController::NomController()
 * @param ssid
 * @param psk
 */
void X7GestConfig::x7GestConfig_setWifi(QString ssid, QString psk)
{
    log4cpp->debug("x7GestConfig_setWifi");
    //Encore une grosse blague ssid= "init" on fait l'init de l'ecran via un signal.
    if (ssid=="init") {
        log4cpp->debug("x7GestConfig_initWifi");
        x7GestConfig_initWifi();
        return;
    }

    //TODO configuration WIFI via connmanctl
}

/**
 * @brief X7GestConfig::x7GestConfig_initWifi
 * - appel signal_ihm_init_wifi()
 */
void X7GestConfig::x7GestConfig_initWifi(void)
{
    log4cpp->debug("x7GestConfig_initWifi");
    QString ssid;QString psk;

    //TODO recupérer la configuration du wifi par connmanctl

    //signal_ihm_init_wifi à corriger et tester
    signal_ihm_init_wifi(ssid,psk);
}

/**
 * @brief X7GestConfig::updateDateTimeNtp
 * - appelé à l'init startX7GestConfig()
 */
void X7GestConfig::updateDateTimeNtp()
{
//Fonction reprise de la T2B
    log4cpp->debug("updateDateTimeNtp");
    int x;
    QChar type;
    QString b;

    if (!serveur_ntp.isEmpty() && !serveur_ntp.contains(' ') )
    {
        log4cpp->debug("ntp not empty");
        if (getIpLocal())
        {
            log4cpp->debug("ip not empty");
            if (!cadence_ntp.isEmpty() && !cadence_ntp.contains(' '))
            {
                log4cpp->debug("cadence not empty");
                if (cadence_ntp.contains('s',Qt::CaseInsensitive)) type='s';
                else if (cadence_ntp.contains('m',Qt::CaseInsensitive)) type='m';
                else if (cadence_ntp.contains('h',Qt::CaseInsensitive)) type='h';
                else type='j';

                b=cadence_ntp;
                b.replace(type,' ',Qt::CaseInsensitive);
                x=b.section(' ',0,0).toInt();
                if (x<=0) x=1;
                switch(type.toLatin1())
                {
                case 's':
                    if (x<=15) x=15;
                    timer_ntp->start(x*1000);     // mise à l'heure tt les x * secondes
                    break;
                case 'm':
                    timer_ntp->start(x*60*1000);     //  mise à l'heure tt les x * minute
                    break;
                case 'h':
                    timer_ntp->start(x*3600*1000);       //  mise à l'heure tt les x * heure
                    break;
                case 'j':
                    timer_ntp->start(24*3600*1000);     // mise à l'heure tt les jour)
                    break;
                }
                //ntp_hwclock_w script qui fait  "ntpd -qnN -p $1" puis "hwclock -w -u"
                //mise à jour datetime par serveur ntp puis sauvegarde dans l'horloge interne t2b.
                QString a=QString("./ntp_hwclock_w %1").arg(serveur_ntp);
                QProcess *qProc = new QProcess(this);
                qProc->startDetached(a);
            }
            else
            {
                timer_ntp->start(24*3600*1000); // tous les jour
            }
        }
        else
        {
            timer_ntp->start(5000);// reessai toutes les 5 secondes
        }
    }

}

/**
 * @brief (NOT_USED) X7GestConfig::x7GestConfig_set_lecteur_badge
 * @param lecteur
 * @param option
 * @param masque
 * @param format
 */
void X7GestConfig::x7GestConfig_set_lecteur_badge(QString lecteur,QString option,QString masque,QString format)
{
    //FIXME utiliser l'objet badge
}

/**
 * @brief (NOT_USED) X7GestConfig::x7GestConfig_init_lecteur_badge
 */
void X7GestConfig::x7GestConfig_init_lecteur_badge(void)
{
    //FIXME utiliser l'objet badge
}

/**
 * @brief X7GestConfig::readConfig
 * - appelé à l'init startX7GestConfig(), dans getIpLocal()
 * - appelé dans X7GestLecteur::initLecteur()
 * - appelé dans X7CarteIO::initCarteIOPort()
 * @return
 */
bool X7GestConfig::readConfig()
{
    log4cpp->debug("readConfig");
    QSqlDatabase bddConfig = QSqlDatabase::database(bddSetupName,false);
    QSqlQuery queryConfig(bddConfig);
    //QSqlQuery queryConfig2(bddConfig); //si besoin d'une requette en parallele.

    queryConfig.clear();
    queryConfig.prepare("SELECT * FROM badge");
    queryConfig.exec();
    if(queryConfig.first())
    {
        log4cpp->debug("Lecture config lecteur badge");
        X7GestLecteur *gestLecteur = X7GestLecteur::getInstance();
        bool *ok;

        //FIXME faire une struc des options de lecteur dans X7GestLecteur et l'utiliser ici.
        gestLecteur->setLecteur(queryConfig.record().value("lecteur").toUInt(ok));
        if(!&ok) {qCritical() << "erreur de conversion X7GestConfig::initConfig Lecteur";}

        gestLecteur->setOption(queryConfig.record().value("option").toUInt(ok));
        if(!&ok) {qCritical() << "erreur de conversion X7GestConfig::initConfig option";}

        gestLecteur->setMasque(queryConfig.record().value("masque").toString());
        gestLecteur->setFormat(queryConfig.record().value("format").toString());
        log4cpp->debug(queryConfig.record().value("lecteur").toString().toStdString() + " - " + queryConfig.record().value("option").toString().toStdString());

    }
    return true;
}

/**
 * @brief (NOT_USED) X7GestConfig::privUpdateDateTime
 * @param dateTime
 */
void X7GestConfig::privUpdateDateTime(const QDateTime *dateTime)
{
    log4cpp->debug("privUpdateDateTime");
    QDateTime now = QDateTime::currentDateTime();
    qDebug() << "Heure system: " << now.toString("dd/MM/yyyy hh:mm:ss - delta ") << (dateTime->toTime_t() - now.toTime_t()) ;

    //Mise a l'heure si +-2 secondes de différence
    if (dateTime->toTime_t() > now.toTime_t()+2 || dateTime->toTime_t() < now.toTime_t()-2)
    {
        qDebug() << "Mise a l'heure : " << dateTime->toString("dd/MM/yyyy hh:mm:ss");
        QProcess proc;
        proc.start("date", QStringList() << "-s" << dateTime->toString("yyyy-MM-dd hh:mm:ss"));
        proc.waitForFinished();
        qDebug() << "date message std : " << proc.readAllStandardOutput();
        qDebug() << "date erreur retour : " << proc.readAllStandardError();
        proc.startDetached("hwclock", QStringList() << "--systohc");
    }
}

/**
 * @brief X7GestConfig::ouvrirBdd
 * - appelé à l'init startX7GestConfig()
 * @param bddName
 * @return
 */
bool X7GestConfig::ouvrirBdd(const QString &bddName)
{
    log4cpp->debug("ouvrirBdd");
    bool ok=false;
    qDebug() << "Reprtoire de l app : " << QDir::currentPath();
    QFile bddFile;
    if(bddName == bddSetupName)
    bddFile.setFileName("/var/x7/etc/database_setup.db");
    else if(bddName == bddBadgeName)
    bddFile.setFileName("/var/x7/contents/database_badge.db");

    if(bddFile.exists())
    {
        QSqlDatabase db = QSqlDatabase::database(bddName,false);
        db.setDatabaseName(bddFile.fileName());

        if(!db.isValid())
        {
            qCritical() << db.lastError().text();
        }else if (!db.open())
        {
            qDebug() << "Impossible d'ouvrir la base de donnée : " << db.databaseName();
        }else
        {
            qDebug() << "Ouverture de la base : " << db.databaseName();
            ok=true;
        }
    }else
    {
        qCritical() << "Fichier de base de données introuvable : \n\t" << QDir::current().filePath(bddFile.fileName());
    }

    return ok;
}

/**
 * @brief (NOT USED) X7GestConfig::fermeBdd
 * @param bddName
 * @return
 */
bool X7GestConfig::fermeBdd(QString bddName)
{
    bool ok=false;

    QSqlDatabase db = QSqlDatabase::database(bddName,false);
    if(!db.isValid())
    {
        qCritical() << db.lastError().text();

    }else if(db.isOpen())
    {
        db.close();
        qDebug() << "Fermeture de la base :" << bddName;
        ok = false;
    }
    return ok;
}

/**
 * @brief (NOT USED) X7GestConfig::customEvent
 * @param event
 */
void X7GestConfig::customEvent(QEvent *event)
{
    log4cpp->debug("customEvent");
    if(X7CustomEvent::custom_type()== event->type())
    {
        X7CustomEvent *e = (X7CustomEvent *)event;

        switch (e->getEventType())
        {

        default:
            break;

        }
    }

}

/**
 * @brief X7GestConfig::x7GestConfig_init_divers
 * - mise à jour table divers
 */
void X7GestConfig::x7GestConfig_init_divers(void)
{
    log4cpp->debug("x7GestConfig_init_divers");
    QString type_ecran="";
    QString backlight_N1="";
    QString delai_N2="";
    QString backlight_N2="";
    QString delai_OFF="";
    QString sleep_mode="";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","divers_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");
    if (db.open())
    {
        log4cpp->debug("Lecture table divers");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM divers",db);
        if (model.rowCount())
        {
            type_ecran=model.record(0).value("ecran").toString();
            backlight_N1=model.record(0).value("backlight N1").toString();
            delai_N2=model.record(0).value("delai N2").toString();
            backlight_N2=model.record(0).value("backlight N2").toString();
            delai_OFF=model.record(0).value("delai OFF").toString();
            sleep_mode=model.record(0).value("sleep mode").toString();
            if(backlight_N1.toInt() < 1) backlight_N1 = "1";
            if(backlight_N1.toInt() > 25) backlight_N1 = "25";
            if(backlight_N2.toInt() < 1) backlight_N2 = "1";
            if(backlight_N2.toInt() > 25) backlight_N2 = "25";
            X7CarteIO::getInstance()->setBrightness(backlight_N1.toInt());
        }else{
            log4cpp->debug("x7GestConfig_init_divers recréation données");
            QSqlQuery query(db);
            query.exec(QString("DELETE from divers"));
            query.clear();
            query.finish();
            db.commit();
            query.prepare("INSERT INTO divers (ecran, 'backlight N1', 'delai N2', 'backlight N2', 'delai OFF', 'sleep mode')" "VALUES (:ecran, :bl1, :delai, :bl2, :delai_off, :sleep_mode);");
            query.bindValue(":ecran", "0");
            query.bindValue(":bl1", "10");
            query.bindValue(":delai", "");
            query.bindValue(":bl2", "");
            query.bindValue(":delai_off", "");
            query.bindValue(":sleep_mode", "");
            query.exec();
            query.finish();
            db.commit();
            db.close();
        }
        model.clear();
        model.deleteLater();
        db.close();
    }
}
