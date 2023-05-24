/**
 * @file
 * @brief Process_qobject and Process_qthread class definition
 * - gestion initialisation matériel (init lecteur, serveur http, serveur ntp,)
*******************************************************************/
//using namespace std;
#include <x7gestlecteur.h>
#include <x7carteio.h>
#include <mainwindow.h>
#include <process.h>
#include <supervision.h>
#include <badge.h>
#include "customnetworkmanagerfactory.h"

// QOBJECT

#include <QtCore/QCoreApplication>

#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>

log4cpp::Category * Process_qobject::log4cpp = &log4cpp::Category::getInstance("Process_qobject");

/**
 * @brief Process_qobject::Process_qobject
 * - Constructeur par defaut.
 */
Process_qobject::Process_qobject(void)
{
    log4cpp->debug("constructeur");
}

/**
 * @brief Process_qobject::process_init
 * - appelé par Process_qthread::run()
 * - Methode qui lance l'initialisation après 1 ms.
 * - appel process_init0()
 */
void Process_qobject::process_init(void)
{
    log4cpp->debug("process_init");
    timer_init =new QTimer();
    timer_init->setSingleShot(true);
    connect(timer_init,SIGNAL(timeout()),this,SLOT(process_init0()));
    timer_init->start(1);
}

/**
 * @brief Process_qobject::process_init0
 * - Methode qui initialise l'objet
 * - creation process_badge_db var/x7/contents/database_badge.db
 * - creation timer_sleep_mode_control, timer_batterie
 * - appel process_init_divers()
 */
void Process_qobject::process_init0(void)
{
    log4cpp->debug("process_init0");
    //supression de l'entrèe de la clé Usb (sda1) si elle existe
    QString a= QString("rm /dev/sda1");
    QProcess *qProc = new QProcess(this);
    qProc->start(a);
    qProc->waitForReadyRead();
    QString tmp = qProc->readAll();
    qProc->close();
    qProc->deleteLater();

    //Initialistation des proprietes
    batterie=0;
    sleep_mode_etat=-1;
    usbkey_mounted=0;

    //FIXME factoriser la gestion de la base de donnée
    //initialisation du lien avec la base de donnée des badges.
    process_badge_db = QSqlDatabase::addDatabase("QSQLITE","process_badge");
    process_badge_db.setDatabaseName("-/var/x7/contents/database_badge.db");


 /*   timer_initprocess=new QTimer();
    timer_initprocess->setSingleShot(true);
    connect(timer_initprocess,SIGNAL(timeout()),this,SLOT(priv_init_process()));
    timer_initprocess->start(1);
*/
 /*   timer_ntp =new QTimer();
    timer_ntp->setSingleShot(true);
    connect(timer_ntp,SIGNAL(timeout()),this,SLOT(priv_refresh_ntp()));
    //timer_ntp->start(60000);
*/

    timer_sleep_mode_control=new QTimer();
    timer_sleep_mode_control->setSingleShot(true);
    connect(timer_sleep_mode_control,SIGNAL(timeout()),this,SLOT(priv_sleep_mode_control()));

    timer_batterie =new QTimer();
    timer_batterie->setSingleShot(true);
    connect(timer_batterie,SIGNAL(timeout()),this,SLOT(priv_refresh_batterie()));
    timer_batterie->start(10000);

    /*
    timer_reboot=new QTimer();
    timer_reboot->setSingleShot(true);
    connect(timer_reboot,SIGNAL(timeout()),this,SLOT(refresh_reboot()));
    //timer_reboot->start(10000);

    timer_rebootauto=new QTimer();
    timer_rebootauto->setSingleShot(true);
    connect(timer_rebootauto,SIGNAL(timeout()),this,SLOT(refresh_rebootauto()));
    //timer_rebootauto->start(60000);

    timer_restartauto=new QTimer();
    timer_restartauto->setSingleShot(true);
    connect(timer_restartauto,SIGNAL(timeout()),this,SLOT(refresh_restartauto()));
    //timer_restartauto->start(80000);

    timer_usbkey=new QTimer();
    timer_usbkey->setSingleShot(true);
    connect(timer_usbkey,SIGNAL(timeout()),this,SLOT(refresh_usbkey()));
    //timer_usbkey->start(4000);
*/
    //modify the port settings on your own
//    port = new QSerialPort("/dev/ttyUSB1");
//    port->setBaudRate(QSerialPort::Baud115200);
//    port->setFlowControl(QSerialPort::NoFlowControl);
//    port->setParity(QSerialPort::NoParity);
//    port->setDataBits(QSerialPort::Data8);
//    port->setStopBits(QSerialPort::TwoStop);
    //port->open(QIODevice::ReadWrite);

    process_init_divers();
}



/**
 * @brief Process_qobject::wake_up
 * - Methode pour sortir du mode veille.
 * - appel X7CarteIO::setBrightness()
 */
void Process_qobject::wake_up(void)
{
    log4cpp->debug("wake_up");
    log4cpp->debug("wake_up batterie = %d", batterie);
    log4cpp->debug("wake_up sleep_mode_etat = %d", sleep_mode_etat);
    if (batterie)
    {
        if (sleep_mode_etat >0)
        {
            timer_sleep_mode_control->stop();
            sleep_mode_etat=-1;

            X7CarteIO::getInstance()->setBrightness(backlight_N1.toInt());

            //QProcess *qProc = new QProcess(this);
            //QString a=QString("./backlight_pwm %1").arg(backlight_N1);
            //qProc->startDetached(a);
            //           int x=((100-backlight_N1.toInt())*255)/100;
            //            QString b=QString("i2cset -y 1 0x60 0x02 0x%1").arg((int) x,0,16);
        }
    }


}



/**
 * @brief Process_qobject::time2bii_tests
 * - Methode pour lancer les tests intégrés
 * - appel test_usb(), test ethernet(), test audio(), test_backlight(), test_sleep_mode()
 * - appel priv_test_lecteur(), priv_test_relay()
 * @param a de 0 à 9 et 99 pour enchainer tous les tests.
 */
void Process_qobject::time2bii_tests(int a)
{
    log4cpp->debug("time2bii_tests");
    //TODO migrer cette methode dans x7gestconfig
    switch (a)
    {
    /*    case 0:
        test_uart("/dev/ttyO0",a);
    break;
*/
    case 0:
        //test_uart("/dev/ttyO1",a);
        priv_test_lecteur(a);
        break;
    case 1:
        //test_uart("/dev/ttyO3",a);
        break;
    case 2:
        test_usb("/sys/bus/usb/devices/1-1",a);
        break;
    case 3:
        //test_usb("/sys/bus/usb/devices/2-1",a);
        break;
    case 4:
        test_ethernet(a);
        break;
    case 5:
        //test_io("0",a);
        priv_test_relay(a);
        break;
    case 6:
        //test_io("1",a);
        break;
    case 7:
        test_backlight(a);
        break;
    case 8:
        test_audio(a);
        break;
    case 9:
        test_sleep_mode(a);
        break;
    default:
        QTime dieTime = QTime::currentTime().addMSecs(500);
        /*        while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        test_uart("/dev/ttyO0",0);
        dieTime = QTime::currentTime().addMSecs(100);
 */
        while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        priv_test_lecteur(0);
        dieTime = QTime::currentTime().addMSecs(100);
        //while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        //test_uart("/dev/ttyO3",1);
        //dieTime = QTime::currentTime().addMSecs(100);
        while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        test_usb("/sys/bus/usb/devices/1-1",2);
        dieTime = QTime::currentTime().addMSecs(100);
        //while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        //test_usb("/sys/bus/usb/devices/2-1",3);
        //dieTime = QTime::currentTime().addMSecs(100);
        while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        test_ethernet(4);
        dieTime = QTime::currentTime().addMSecs(100);
        while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        priv_test_relay(5);
        dieTime = QTime::currentTime().addMSecs(100);
        //while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        //test_io("1",6);
        //dieTime = QTime::currentTime().addMSecs(100);
        while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        test_backlight(7);
        dieTime = QTime::currentTime().addMSecs(100);
        while( QTime::currentTime() < dieTime ) QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
        test_audio(8);
        break;

    }
}

//FUNCTION time2bii_tests
/**
 * @brief (NOT USED) Process_qobject::test_uart
 * - Methode de test d'un port serie
 * @param port chemin du port serie Ex: /dev/ttyO1
 * @param a indice du port pour le retour de l'IHM
 */
void Process_qobject::test_uart(QString port,int a)
{
    log4cpp->debug("test_uart");
    //TODO migrer cette methode dans x7gestconfig
    //TODO BG supprimer le parametre a et faire autrement.
    emit signal_ihm_retour_test(2,a);
    delayBackLight();
    int i=0;
    port_test = new QSerialPort(port);

    port_test->setBaudRate(QSerialPort::Baud115200);
    port_test->setFlowControl(QSerialPort::NoFlowControl);
    port_test->setParity(QSerialPort::NoParity);
    port_test->setDataBits(QSerialPort::Data8);
    port_test->setStopBits(QSerialPort::TwoStop);
    port_test->open(QIODevice::ReadWrite);
    message="ceci est un message ";
    received_msg.clear();
    /*appel priv_transmitMsg */
    priv_transmitMsg(port_test);
    /*appel priv_receiveMsg */
    priv_receiveMsg(port_test);
    if (!received_msg.compare(message)) i=1;
    emit signal_ihm_retour_test(i,a);

    port_test->close();
}

/**
 * @brief Process_qobject::test_usb
 * - Methode de test d'un port Usb
 * @param port chemin du port Usb Ex: /sys/bus/usb/devices/1-1
 * @param a indice du port pour le retour de l'IHM
 */
void Process_qobject::test_usb(QString port,int a)
{
    log4cpp->debug("test_usb");
    //TODO migrer cette methode dans x7gestconfig
    emit signal_ihm_retour_test(2,a);
    delayBackLight();
    QDir dir(port);
    if (dir.exists()) emit signal_ihm_retour_test(1,a);
    else emit signal_ihm_retour_test(0,a);
}

/**
 * @brief Process_qobject::test_ethernet
 * - Methode de test de l'interface ethernet
 * @param a
 */
void Process_qobject::test_ethernet(int a)
{
    log4cpp->debug("test_ethernet");
    // test interface OK
    //TODO migrer cette methode dans x7gestconfig
    emit signal_ihm_retour_test(2,a);
    delayBackLight();
    QList<QNetworkInterface> interfaces ;
    QString interface_name;
    interface_name.clear();
    interfaces = QNetworkInterface::allInterfaces();
    foreach(const QNetworkInterface & interface, interfaces)
    {
        if (interface.name()=="eth0")
        {
            if (interface.flags() & QNetworkInterface::IsUp)
            {
                if (interface.flags()  & QNetworkInterface::IsRunning)
                {
                    emit signal_ihm_retour_test(1,a);
                }
                else emit signal_ihm_retour_test(0,a);
            }
            else emit signal_ihm_retour_test(0,a);
        }
    }
}

/**
 * @brief (NOT USED) Process_qobject::test_io
 * @param test
 * @param a
 */
void Process_qobject::test_io(QString test,int a)
{
    log4cpp->debug("test_io");
    //TODO migrer cette methode dans x7gestconfig
    emit signal_ihm_retour_test(2,a);
    delayBackLight();
    QString s,value1,value2,value3,value4;
    QProcess *qProc = new QProcess(this);
    /*    s=QString("rmmod data_clock");
    qProc->start(s);
    qProc->waitForReadyRead();
    qProc->readAll();
    qProc->close();
    s=QString("rmmod wiegand");
    qProc->start(s);
    qProc->waitForReadyRead();
    qProc->readAll();
    qProc->close();
  */
    s=QString("./test_io_shell");
    qProc->start(s);
    qProc->waitForReadyRead();
    qProc->readAll();
    qProc->close();
    s=QString("cat /sys/class/gpio/gpio82/value");
    qProc->start(s);
    qProc->waitForReadyRead();
    value1=qProc->readAll();
    value1.truncate(1);
    qProc->close();
    s=QString("cat /sys/class/gpio/gpio106/value");
    qProc->start(s);
    qProc->waitForReadyRead();
    value2=qProc->readAll();
    value2.truncate(1);
    qProc->close();
    s=QString("cat /sys/class/gpio/gpio13/value");
    qProc->waitForReadyRead();
    value3=qProc->readAll();
    value3.truncate(1);
    qProc->close();
    s=QString("cat /sys/class/gpio/gpio12/value");
    qProc->start(s);
    qProc->waitForReadyRead();
    value4=qProc->readAll();
    value4.truncate(1);
    qProc->close();
    if (!value1.compare(test) && !value2.compare(test) && !value3.compare(test) && !value4.compare(test)) emit signal_ihm_retour_test(1,a);
    else emit signal_ihm_retour_test(0,a);

    qProc->deleteLater();
}

/**
 * @brief Process_qobject::test_audio
 * - Methode de test de l'interface audio
 * @param a
 */
void Process_qobject::test_audio(int a)
{
    log4cpp->debug("test_audio");
    //TODO migrer cette methode dans x7gestconfig
    emit signal_ihm_retour_test(2,a);
    delayBackLight();
    QString b=QString("aplay /var/x7/contents/wav/badge_ok.wav");
    QProcess *qProc = new QProcess(this);
    qProc->startDetached(b);
    emit signal_ihm_retour_test(1,a);
}

/**
 * @brief Process_qobject::test_backlight
 * - Methode de test du retroéclairage
 * @param a
 */
void Process_qobject::test_backlight(int a)
{
    log4cpp->debug("test_backlight");
    //TODO migrer cette methode dans x7gestconfig
    emit signal_ihm_retour_test(2,a);
    X7CarteIO::getInstance()->setBrightness(1);
    delayBackLight();
    X7CarteIO::getInstance()->setBrightness(5);
    delayBackLight();
    X7CarteIO::getInstance()->setBrightness(10);
    delayBackLight();
    X7CarteIO::getInstance()->setBrightness(15);
    delayBackLight();
    X7CarteIO::getInstance()->setBrightness(20);
    delayBackLight();
    X7CarteIO::getInstance()->setBrightness(25);
    emit signal_ihm_retour_test(1,a);
}

/**
 * @brief Process_qobject::test_sleep_mode
 * - Methode de test du mode veille
 * @param a
 */
void Process_qobject::test_sleep_mode(int a)
{
    log4cpp->debug("test_sleep_mode");
    //TODO supprimer cette methode
    return;
    QString b=QString("./sleep_mode");
    QProcess *qProc = new QProcess(this);
    qProc->startDetached(b);
    emit signal_ihm_retour_test(1,a);

}

//END FUNCTION time2bii_tests

/**
 * @brief Process_qobject::process_set_lecteur_badge
 * - Methode qui fait l'initialisation du lecteur
 * - appel process_init_lecteur_badge() à l'init
 * - appel signal_X7GestLecteur_initLecteur()
 * @param lecteur
 * @param option
 * @param masque
 * @param format
 */
void Process_qobject::process_set_lecteur_badge(QString lecteur,QString option,QString masque,QString format)
{
    log4cpp->debug("process set_lecteur_badge");
    // HA HA HA je suis mort de rire la chaine "init" est envoyé par l'UI apres un timer pour s'initialiser.
    //FIXME OR DIE
    //TODO migrer cette methode dans x7gestconfig
    if (lecteur=="init") {process_init_lecteur_badge();return;}

    //à l'init, valeur lecteur recupéré dans database_setup.db (=0 par défaut)
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","badge_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");

    if (badge_thread.badge.lecteur.compare(lecteur) || badge_thread.badge.option.compare(option) ||
            badge_thread.badge.masque.compare(masque) || badge_thread.badge.format.compare(format))
    {
        if (db.open())
        {
            log4cpp->debug("Recreation enregistrement badge");
            QSqlQuery query(db);
            query.exec(QString("DELETE from badge"));
            query.clear();
            query.finish();
            db.commit();
            query.prepare("INSERT INTO badge (lecteur, option, masque, format)" "VALUES (:lecteur, :option, :masque, :format);");
            query.bindValue(":lecteur", lecteur);
            query.bindValue(":option", option);
            query.bindValue(":masque", masque);
            query.bindValue(":format", format);
            query.exec();
            query.clear();
            query.finish();
            db.commit();
            db.close();
            //appel X7GestLecteur function initLecteur
            signal_X7GestLecteur_initLecteur();

        }
    }
}

/**
 * @brief Process_qobject::process_init_lecteur_badge
 * - Acces à database_setup.db
 * - appel signal_ihm_init_lecteur_badge() pour affichage des infos du lecteur dans l'IHM
 */
void Process_qobject::process_init_lecteur_badge(void)
{
    log4cpp->debug("process_init_lecteur_badge");
    //FIXME factoriser acces BDD
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","badge_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");
    QString lecteur;QString option;QString masque;QString format;

    lecteur.clear();
    option.clear();
    masque.clear();
    format.clear();

    if (db.open())
    {
        log4cpp->debug("Lecture table badge process.");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM badge",db);
        if (model.rowCount())
        {
            lecteur=model.record(0).value("lecteur").toString();
            option=model.record(0).value("option").toString();
            masque=model.record(0).value("masque").toString();
            format=model.record(0).value("format").toString();
        }
        model.clear();
        model.deleteLater();
        db.close();
    }

    if (lecteur.isEmpty()) lecteur="0";
    if (option.isEmpty()) option="0";

    /* appel mainView.qml init_lecteur_badge pour affichage info lecteur */
    signal_ihm_init_lecteur_badge(lecteur,option,format,masque);

}

/**
 * @brief (NOT_USED) Process_qobject::process_set_nom
 * - appel process_init_mode_network()
 * @param nom
 */
void Process_qobject::process_set_nom(QString nom)
{
    log4cpp->debug("set_nom");
    QString image;
    image.append(nom);
    //TODO migrer cette methode dans x7gestconfig
    //fixme
    return;
    if (!nom.isEmpty())
    {
        // --- Initialisation du hostname (fichier /etc/hostname + commande)
        if (!nom.contains("init_nom"))
        {
            nom.append('\n');
            QFile out("/etc/hostname");
            if (out.open( QFile::WriteOnly))
            {
                QTextStream stream_out(&out);
                stream_out << nom;
                stream_out.flush();
                out.close();
            }
        }

        QProcess *qProc2 = new QProcess(this);
        qProc2->start("hostname -F /etc/hostname");
        qProc2->waitForReadyRead();
        qProc2->readAll();
        qProc2->close();
        qProc2->deleteLater();
        // --- verification ---
        QProcess *qProc3 = new QProcess(this);
        qProc3->start("hostname");
        qProc3->waitForReadyRead();
        QByteArray x=qProc3->readAll();
        QString time2bii_name=QString::fromLatin1(x.data());
        supervision_thread.supervision.time2bii_name=time2bii_name.rightJustified((time2bii_name.length())-1,'\0',true);
        qProc3->close();
        qProc3->deleteLater();

        QFile in_out("/etc/network/interfaces");
        QFile in("../etc/interfaces.dhcpinit");
        QFile out("../etc/interfaces.dhcp");
        if (in_out.open( QFile::ReadWrite))
        {
            QTextStream stream_in_out(&in_out);
            QString s=stream_in_out.readAll();
            if (s.contains("dhcp"))
            {
                if (in.open( QFile::ReadOnly))
                {
                    QTextStream stream_in(&in);
                    QTextStream stream_out(&out);
                    s.clear();
                    s=stream_in.readAll();
                    QString a=QString("hostname %1").arg(time2bii_name);
                    s.replace("hostname",a);
                    out.open( QFile::WriteOnly);
                    stream_out << s;
                    stream_out.flush();
                    stream_in_out.seek(0);
                    stream_in_out << s;
                    stream_in_out.flush();
                    in.close();
                    out.close();
                }
                QProcess *qProc = new QProcess(this);
                QString b= QString("ip route del");
                qProc->start(b);
                qProc->waitForReadyRead();
                qProc->readAll();
                b= QString("/etc/init.d/networking restart");
                qProc->start(b);
                QTest::qWait(1000);
                while ( qProc->state()==QProcess::Running) QTest::qWait(100);
                qProc->close();
                qProc->deleteLater();
                //QTest::qWait(4000);
                if (!image.contains("init_nom")) process_init_mode_network();
            }
            in_out.close();
        }
    }
}


//FIXME cette methode est foireuse !!! a remplacer par l'appel de la commande connman.
/**
 * @brief (NOT USED) Process_qobject::process_set_network
 * - appel process_init_mode_network()
 * @param interface
 * @param ip
 * @param netmask
 * @param gateway
 */
void Process_qobject::process_set_network(QString interface,QString ip,QString netmask,QString gateway)
{
    log4cpp->debug("set_network");
    QProcess *qProc = new QProcess(this);
    QString a,b;

    //fixme
    return;

    if(interface.contains("DHCP") || interface.contains("dhcp")
            || ip.isEmpty() || ip.count(QRegExp("[0-9]{1,1}[.][0-9]{1,1}"))!=3 || ip.section('.',0,0).length() >3 ||  ip.section('.',1,1).length() >3
            ||  ip.section('.',2,2).length() >3  || ip.section('.',3,3).length() >3
            || netmask.isEmpty() || netmask.count(QRegExp("[0-9]{1,1}[.][0-9]{1,1}"))!=3 || netmask.section('.',0,0).length() >3 ||  netmask.section('.',1,1).length() >3
            ||  netmask.section('.',2,2).length() >3  || netmask.section('.',3,3).length() >3
            || (!gateway.isEmpty() && (gateway.count(QRegExp("[0-9]{1,1}[.][0-9]{1,1}")) !=3 || gateway.section('.',0,0).length() >3 ||  gateway.section('.',1,1).length() >3
                                       ||  gateway.section('.',2,2).length() >3  || gateway.section('.',3,3).length() >3)))
    {
        b= QString("chmod 666 /etc/network/if-up.d/udhcp");
        qProc->start(b);
        qProc->waitForReadyRead();
        qProc->readAll();
        b= QString("chmod 666 /etc/network/if-up.d/wnameserver");
        qProc->start(b);
        qProc->waitForReadyRead();
        qProc->readAll();
        b= QString("cp ../etc/interfaces.dhcp /etc/network/interfaces");
        qProc->start(b);
        qProc->waitForReadyRead();
        qProc->readAll();
        b= QString("sync");
        qProc->start(b);
        qProc->waitForReadyRead();
        qProc->readAll();
        // a= QString("./dhcpon 1");/// en dhcp
    }
    else /// en statique
    {

        QFile in("../etc/interfaces.cfg");
        QFile out("/etc/network/interfaces");
        if (in.open( QFile::ReadOnly))
        {
            b= QString("chmod 666 /etc/network/if-up.d/udhcp");
            qProc->start(b);
            qProc->waitForReadyRead();
            qProc->readAll();
            b= QString("chmod 777 /etc/network/if-up.d/wnameserver");
            qProc->start(b);
            qProc->waitForReadyRead();
            qProc->readAll();
            QTextStream stream_in(&in);
            QTextStream stream_out(&out);
            QString s=stream_in.readAll();
            a=QString("address %1").arg(ip);
            s.replace("address",a);
            a=QString("netmask %1").arg(netmask);
            s.replace("netmask",a);
            a=QString("gateway %1").arg(gateway);
            if (gateway.isEmpty()) s.replace("gateway","");
            else s.replace("gateway",a);
            //            log4cpp->debug("Gateway "+ s);
            out.open( QFile::WriteOnly);
            stream_out << s;
            stream_out.flush();
            in.close();
            out.close();
        }
    }
    b= QString("ip route del");
    qProc->start(b);
    qProc->waitForReadyRead();
    qProc->readAll();
    b= QString("/etc/init.d/networking restart");
    qProc->start(b);
    QTest::qWait(1000);
    while ( qProc->state()==QProcess::Running) QTest::qWait(100);
    qProc->close();
    qProc->deleteLater();
    //QTest::qWait(4000);
    process_init_mode_network();
}


//FIXME cette methode est foireuse !!! a remplacer par l'appel de la commande connman.
/**
 * @brief (NOT USED) Process_qobject::process_set_mode_network
 * - appel process_init_mode_network()
 * - appel process_init_divers()
 * @param mode
 * @param nom
 * @param ip
 * @param netmask
 * @param gateway
 * @param dns1
 * @param dns2
 */
void Process_qobject::process_set_mode_network(int mode,QString nom,QString ip,QString netmask,QString gateway,QString dns1,QString dns2)
{
    log4cpp->debug("FIXME set_mode_network");
    //mode -1 init, 0 dhcp, 1 ipfix
    return;

    if (mode <0) { process_init_mode_network();return;}

    QProcess *qProc = new QProcess(this);
    QString a,b;


    if (!nom.isEmpty())
    {
        // --- Initialisation du hostname (fichier /etc/hostname + commande)
        QFile out("/etc/hostname");
        if (out.open( QFile::WriteOnly))
        {
            QTextStream stream_out(&out);
            stream_out << nom;
            stream_out.flush();
            out.close();
        }
        QProcess *qProc2 = new QProcess(this);
        qProc2->start("hostname -F /etc/hostname");
        qProc2->waitForReadyRead();
        qProc2->readAll();
        qProc2->close();
        qProc2->deleteLater();
        // --- verification ---
        QProcess *qProc3 = new QProcess(this);
        qProc3->start("hostname");
        qProc3->waitForReadyRead();
        QByteArray x=qProc3->readAll();
        QString time2bii_name=QString::fromLatin1(x.data());
        supervision_thread.supervision.time2bii_name=time2bii_name.rightJustified((time2bii_name.length())-1,'\0',true);
        qProc3->close();
        qProc3->deleteLater();

        QFile in("../etc/interfaces.dhcpinit");
        out.setFileName("../etc/interfaces.dhcp");
        if (in.open( QFile::ReadOnly))
        {
            QTextStream stream_in(&in);
            QTextStream stream_out(&out);
            QString s=stream_in.readAll();
            a=QString("hostname %1").arg(nom);
            s.replace("hostname",a);
            out.open( QFile::WriteOnly);
            stream_out << s;
            stream_out.flush();
            in.close();
            out.close();
        }
    }


    if(!mode)
    {
        b= QString("chmod 666 /etc/network/if-up.d/udhcp");
        qProc->start(b);
        qProc->waitForReadyRead();
        qProc->readAll();
        b= QString("chmod 666 /etc/network/if-up.d/wnameserver");
        qProc->start(b);
        qProc->waitForReadyRead();
        qProc->readAll();
        b= QString("cp ../etc/interfaces.dhcp /etc/network/interfaces");
        qProc->start(b);
        qProc->waitForReadyRead();
        qProc->readAll();
        b= QString("sync");
        qProc->start(b);
        qProc->waitForReadyRead();
        qProc->readAll();

        // a= QString("./dhcpon 1");/// en dhcp
    }
    else /// en statique
    {
        // --- verifier format @ IP
        if (ip.count(QRegExp("[0-9]{1,1}[.][0-9]{1,1}"))!=3 || ip.section('.',0,0).length() >3 ||  ip.section('.',1,1).length() >3
                ||  ip.section('.',2,2).length() >3  || ip.section('.',3,3).length() >3)
            ip="192.168.1.2";

        if (netmask.count(QRegExp("[0-9]{1,1}[.][0-9]{1,1}"))!=3 || netmask.section('.',0,0).length() >3 ||  netmask.section('.',1,1).length() >3
                ||  netmask.section('.',2,2).length() >3  || netmask.section('.',3,3).length() >3) netmask="255.255.255.0";

        if (!gateway.isEmpty() && (gateway.count(QRegExp("[0-9]{1,1}[.][0-9]{1,1}")) !=3 || gateway.section('.',0,0).length() >3 ||  gateway.section('.',1,1).length() >3
                                   ||  gateway.section('.',2,2).length() >3  || gateway.section('.',3,3).length() >3)) gateway.clear();

        if (!dns1.isEmpty() && (dns1.count(QRegExp("[0-9]{1,1}[.][0-9]{1,1}")) !=3 || dns1.section('.',0,0).length() >3 ||  dns1.section('.',1,1).length() >3
                                ||  dns1.section('.',2,2).length() >3  || dns1.section('.',3,3).length() >3)) dns1.clear();

        if (!dns2.isEmpty() && (dns2.count(QRegExp("[0-9]{1,1}[.][0-9]{1,1}")) !=3 || dns2.section('.',0,0).length() >3 ||  dns2.section('.',1,1).length() >3
                                ||  dns2.section('.',2,2).length() >3  || dns2.section('.',3,3).length() >3)) dns2.clear();

        QFile in("../etc/interfaces.cfg");
        QFile out("/etc/network/interfaces");
        if (in.open( QFile::ReadOnly))
        {
            b= QString("chmod 666 /etc/network/if-up.d/udhcp");
            qProc->start(b);
            qProc->waitForReadyRead();
            qProc->readAll();
            b= QString("chmod 777 /etc/network/if-up.d/wnameserver");
            qProc->start(b);
            qProc->waitForReadyRead();
            qProc->readAll();
            QTextStream stream_in(&in);
            QTextStream stream_out(&out);
            QString s=stream_in.readAll();
            a=QString("address %1").arg(ip);
            s.replace("address",a);
            a=QString("netmask %1").arg(netmask);
            s.replace("netmask",a);
            a=QString("gateway %1").arg(gateway);
            if (gateway.isEmpty()) s.replace("gateway","");
            else s.replace("gateway",a);
            out.open( QFile::WriteOnly);
            stream_out << s;
            stream_out.flush();
            in.close();
            out.close();
        }

        if (!dns1.isEmpty() || !dns2.isEmpty())
        {    process_init_divers();

            QFile outdns("/etc/resolv.conf");
            QTextStream stream_out(&outdns);
            a.clear();
            if (!dns1.isEmpty()) a.append("nameserver "+dns1+'\n');
            if (!dns2.isEmpty()) a.append("nameserver "+dns2+'\n');
            outdns.open( QFile::WriteOnly);
            stream_out << a;
            stream_out.flush();
            outdns.close();
        }

    }
    b= QString("ip route del");
    qProc->start(b);
    qProc->waitForReadyRead();
    qProc->readAll();
    b= QString("/etc/init.d/networking restart");
    qProc->start(b);
    QTest::qWait(1000);
    while ( qProc->state()==QProcess::Running) QTest::qWait(100);
    qProc->close();
    qProc->deleteLater();
}


//FIXME cette methode est foireuse !!! a remplacer par l'appel de la commande connman.
/**
 * @brief (NOT USED) Process_qobject::process_init_mode_network
 * - appel signal_init_mode_network() (obsolete)
 */
void Process_qobject::process_init_mode_network(void)
{
    log4cpp->debug("process_init_mode_network");
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
    //obsolete
    //signal_init_mode_network(nom,ip.toString(),netmask.toString(),gateway.toString(),dns1,dns2);
}

/**
 * @brief Process_qobject::process_set_http
 * - Methode pour verifier la connexion au serveur chronotime et l'affichage sur IHM
 * - appel process_init_http() et lecture database_setub.db
 * - appel signal_supervision_init_http()
 * @param serveur
 * @param https
 * @param http
 * @param service
 * @param user
 * @param password
 * @param timeout
 */
void Process_qobject::process_set_http(QString serveur,QString https,QString http,QString service,QString user,QString password,QString timeout)
{
    log4cpp->debug("process set_http");
    if (serveur=="init") {process_init_http();return;}

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","supervision_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");

    if (timeout.toInt() <2 ) timeout="2";

    if (supervision_thread.supervision.http_server.compare(serveur) || supervision_thread.supervision.http_port_https.compare(https) ||
            supervision_thread.supervision.http_port_http.compare(http) || supervision_thread.supervision.http_service.compare(service) ||
            supervision_thread.supervision.http_user.compare(user) || supervision_thread.supervision.http_password.compare(password) || supervision_thread.supervision.http_timeout.compare(timeout))
    {
        if (db.open())
        {
            log4cpp->debug("Recreation enregistrement http");
            QSqlQuery query(db);
            query.exec(QString("DELETE from http"));
            query.clear();
            query.finish();
            db.commit();
            query.prepare("INSERT INTO http (serveur, 'port https', 'port http', user, password, service, timeout)" "VALUES (:serveur, :https, :http, :user, :password, :service, :timeout);");
            query.bindValue(":serveur", serveur);
            query.bindValue(":https", https);
            query.bindValue(":http", http);
            query.bindValue(":user", user);
            query.bindValue(":password", password);
            query.bindValue(":service", service);
            query.bindValue(":timeout", timeout);
            query.exec();
            query.finish();
            db.commit();
            db.close();

            //FIXME set les variables dans l'objet  supervision;
            supervision_thread.supervision.http_server = serveur;
            supervision_thread.supervision.http_port_https = https;
            supervision_thread.supervision.http_port_http =http;
            supervision_thread.supervision.http_service = service;
            supervision_thread.supervision.http_user = user;
            supervision_thread.supervision.http_password = password;
            supervision_thread.supervision.http_timeout = timeout;
        }

        //            log4cpp->debug(" RESTART SUPERVISION");
        signal_supervision_init_http(); //Connexion serveur http/https (chronotime) et suppression logo alerte_http si pas d'erreur
        signal_supervision_http_status(1);
    }
}

/**
 * @brief Process_qobject::process_init_http
 * - Methode pour initialiser la connexion au serveur chronotime
 * - appel signal_ihm_init_http()
 */
void Process_qobject::process_init_http(void)
{
    log4cpp->debug("process init_http");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","supervision_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");
    QString serveur;QString https;QString http;QString service;QString user;QString password;QString timeout;

    if (db.open())
    {
        QSqlQueryModel model;

        model.setQuery("SELECT * FROM http",db);
        if (!model.record(0).contains("timeout"))
        {
            log4cpp->debug("AlterTable http");
            model.setQuery("ALTER TABLE http ADD timeout VARCHAR(5)",db);
            db.commit();
            model.setQuery("UPDATE http SET timeout = '5' ",db);
            db.commit();
        }

        model.setQuery("SELECT * FROM http",db);
        if (model.rowCount())
        {
            supervision_thread.supervision.http_server = serveur = model.record(0).value("serveur").toString();
            supervision_thread.supervision.http_port_https = https = model.record(0).value("port https").toString();
            supervision_thread.supervision.http_port_http = http = model.record(0).value("port http").toString();
            supervision_thread.supervision.http_service = service = model.record(0).value("service").toString();
            supervision_thread.supervision.http_user = user = model.record(0).value("user").toString();
            supervision_thread.supervision.http_password = password = model.record(0).value("password").toString();
            supervision_thread.supervision.http_timeout = timeout = model.record(0).value("timeout").toString();
            log4cpp->debug("mise a jour http_server: "+ model.record(0).value("serveur").toString().toStdString());
        }

        if (timeout.toInt() < 2)
        {
            log4cpp->debug("Update http");
            model.setQuery("UPDATE http SET timeout = '2' ",db);
            db.commit();
            supervision_thread.supervision.http_timeout=timeout='2';
        }
        model.clear();
        model.deleteLater();
        db.close();
    }
    // appel mainView.qml ihm_init_http
    emit signal_ihm_init_http(serveur,https,http,service,user,password,timeout);
}



/**
 * @brief Process_qobject::process_set_ntp
 * - appel process_init_ntp() et lecture database_setup.db
 * - appel priv_set_timezone_langue()
 * @param serveur
 * @param cadence
 * @param timezone
 * @param langue
 */
void Process_qobject::process_set_ntp(QString serveur,QString cadence,QString timezone,QString langue)
{
    log4cpp->debug("process set_ntp");
    if (serveur=="init") {process_init_ntp();return;}
    QString serveur_base;QString cadence_base;QString timezone_base;QString langue_base;

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","ntp_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");


    if (db.open())
    {
        log4cpp->debug("Lecture table ntp(set)");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM ntp",db);

        if (model.rowCount())
        {
            serveur_base=model.record(0).value("serveur").toString();
            cadence_base=model.record(0).value("cadence").toString();
            timezone_base=model.record(0).value("timezone").toString();
            langue_base=model.record(0).value("langue").toString();
        }
        model.clear();
        model.deleteLater();
        db.close();
    }

    if(serveur_base.compare(serveur) || cadence_base.compare(cadence) || timezone_base.compare(timezone) || langue_base.compare(langue))
    {
        log4cpp->debug("Lecture table ntp(set)");
        if (db.open())
        {
            log4cpp->debug("Recreation enregistrement ntp");
            QSqlQuery query(db);
            query.exec(QString("DELETE from ntp"));
            query.clear();
            query.finish();
            db.commit();
            query.prepare("INSERT INTO ntp (serveur, cadence, timezone, langue)" "VALUES (:serveur, :cadence, :timezone, :langue);");
            query.bindValue(":serveur", serveur);
            query.bindValue(":cadence", cadence);
            query.bindValue(":timezone", timezone);
            query.bindValue(":langue", langue);
            query.exec();
            query.finish();
            db.commit();
            db.close();
            /* appel private priv_set_timezone_langue */
            priv_set_timezone_langue(0,timezone,langue);
        }

    }
}


/**
 * @brief Process_qobject::process_init_ntp
 *  - Methode init date heure
 *  - lecture database_detup.db
 *  - mise à jour /etc/profile.d/timezone_langue.sh
 *  - appel signal_ihm_init_ntp()
 */
void Process_qobject::process_init_ntp(void)
{
    log4cpp->debug("process_init_ntp");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","ntp_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");
    //QString serveur;QString cadence;
    QString timezone;QString langue;

    serveur_ntp.clear();
    cadence_ntp.clear();
    timezone.clear();
    langue.clear();

    if (db.open())
    {
        log4cpp->debug("Lecture table ntp(init)");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM ntp",db);

        if (!model.record(0).contains("timezone"))
        {
            log4cpp->debug("AlterTable ntp");
            model.setQuery("ALTER TABLE ntp ADD timezone VARCHAR(60)",db);
            db.commit();
            model.setQuery("UPDATE ntp SET timezone = 'Europe/Paris'",db);
            db.commit();
            model.setQuery("ALTER TABLE ntp ADD langue VARCHAR(10)",db);
            db.commit();
            model.setQuery("UPDATE ntp SET langue = 'fr_FR'",db);
            db.commit();
        }

        log4cpp->debug("count :" + model.rowCount());
        if (model.rowCount())
        {
            log4cpp->debug("serveur valid :" + model.record(0).value("serveur").isValid());

            serveur_ntp=model.record(0).value("serveur").toString();
            cadence_ntp=model.record(0).value("cadence").toString();
            timezone=model.record(0).value("timezone").toString();
            langue=model.record(0).value("langue").toString();
        }
        model.clear();
        model.deleteLater();
        db.close();


        QFile in("/etc/profile.d/timezone_langue.sh");
        if (in.open( QFile::ReadOnly))
        {
            QTextStream stream_in(&in);
            QString s=stream_in.readAll();
            in.close();
            QString a=QString("TZ=%1").arg('"'+timezone+'"');
            QString b=QString("LC_TIME=%1").arg(langue+'\n');
            if (!s.contains(a) || !s.contains(b)) priv_set_timezone_langue(1,timezone,langue);
        }
    }
    //appel mainView signal_ihm_init_ntp
    signal_ihm_init_ntp(serveur_ntp,cadence_ntp,timezone,langue);
}



/**
 * @brief Process_qobject::process_set_divers
 * - appel process_init_divers()
 * - appel signal_window_init_orientation_ecran()

 * @param ecran
 * @param bl1
 * @param delai
 * @param bl2
 * @param delai_off
 * @param sleep_mode1
 */
void Process_qobject::process_set_divers(QString ecran,QString bl1,QString delai,QString bl2,QString delai_off,QString sleep_mode1)
{
    log4cpp->debug("process_set_divers");
    if (ecran=="init") {process_init_divers();return;}

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","supervison_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");

    if(type_ecran.compare(ecran) || backlight_N1.compare(bl1) || delai_N2.compare(delai)
            || backlight_N2.compare(bl2) || delai_OFF.compare(delai_off) || sleep_mode.compare(sleep_mode1))
    {
        log4cpp->debug("set divers initialisation");
        type_ecran=ecran;
        backlight_N1=bl1;
        delai_N2=delai;
        backlight_N2=bl2;
        delai_OFF=delai_off;
        sleep_mode=sleep_mode1;
        if(bl1.toInt() < 1) bl1 = "1";
        if(bl1.toInt() > 25) bl1 = "25";
        if(bl2.toInt() < 1) bl2 = "1";
        if(bl2.toInt() > 25) bl2 = "25";
        if (db.open())
        {
            QSqlQuery query(db);
            query.exec(QString("DELETE from divers"));
            query.clear();
            query.finish();
            db.commit();
            query.prepare("INSERT INTO divers (ecran, 'backlight N1', 'delai N2', 'backlight N2', 'delai OFF', 'sleep mode')" "VALUES (:ecran, :bl1, :delai, :bl2, :delai_off, :sleep_mode);");
            query.bindValue(":ecran", type_ecran);
            query.bindValue(":bl1", backlight_N1);
            query.bindValue(":delai", delai_N2);
            query.bindValue(":bl2", backlight_N2);
            query.bindValue(":delai_off", delai_off);
            query.bindValue(":sleep_mode", sleep_mode);
            query.exec();
            query.finish();
            db.commit();
            db.close();
        }
    }
    signal_window_init_orientation_ecran(ecran);
    X7CarteIO::getInstance()->setBrightness(backlight_N1.toInt());
}

/**
 * @brief Process_qobject::process_init_divers
 * - lecture table divers
 * - appel signal_ihm_init_divers() pour affichage IHM
 */
void Process_qobject::process_init_divers(void)
{
    log4cpp->debug("process_init_divers");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","divers_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");
    //    QString bl1;QString delai;QString bl2;QString delai_off;QString sleep_mode;

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
            log4cpp->debug("process_init_divers recréation données");
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
        log4cpp->debug("Fin table divers");
    }
    /* appel mainView.qml signal_ihm_init_divers() */
    signal_ihm_init_divers(type_ecran,backlight_N1,delai_N2,backlight_N2,delai_OFF,sleep_mode);

}

/**
 * @brief (NOT USED) Process_qobject::process_set_wifi
 * @param ssid
 * @param psk
 */
void Process_qobject::process_set_wifi(QString ssid, QString psk)
{
    log4cpp->debug("process_set_wifi");
    if (ssid=="init") {process_init_wifi();return;}
    QProcess *qProc = new QProcess(this);
    QString a,b;

    QFile in("wpa_supplicant.cfg");
    QFile out("/etc/wpa_supplicant.conf");
    if (in.open( QFile::ReadOnly))
    {
        QTextStream stream_in(&in);
        QTextStream stream_out(&out);
        QString s=stream_in.readAll();
        QString c=s.section("network={",1,1);
        s.remove(c);
        a=QString("ssid=%1").arg('"'+ssid+'"');
        b=QString("psk=%1").arg('"'+psk)+'"';
        s.append('\n');
        s.append(a);
        s.append('\n');
        s.append(b);
        s.append('\n');
        s.append('}');

        out.open( QFile::WriteOnly);
        stream_out << s;
        stream_out.flush();
        in.close();
        out.close();
    }

    b= QString("ip route del");
    qProc->start(b);
    qProc->waitForReadyRead();
    qProc->readAll();
    b= QString("/etc/init.d/networking restart");
    qProc->start(b);
    QTest::qWait(1000);
    while ( qProc->state()==QProcess::Running) QTest::qWait(100);
    qProc->close();
    qProc->deleteLater();
}

/**
 * @brief (NOT USED) Process_qobject::process_init_wifi
 */
void Process_qobject::process_init_wifi(void)
{
    log4cpp->debug("process_init_wifi");
    QString ssid;QString psk;
    ssid.clear();
    psk.clear();

    QFile in("/etc/wpa_supplicant.conf");
    if (in.open( QFile::ReadOnly))
    {
        QTextStream stream_in(&in);
        QString s;
        do{
            s = stream_in.readLine();
            if (s.contains("ssid=")) ssid=s.section('"',1,1);
            if (s.contains("psk=")) psk=s.section('"',1,1);
        } while (!s.isNull());

        in.close();
    }
    //  ssid.remove('"');
    // psk.remove('"');

    //signal_init_wifi(ssid,psk);//obsolete
}

/**
 * @brief Process_qobject::process_set_3G
 * @param apn
 * @param pin
 * @param phone
 * @param user
 * @param password
 */
void Process_qobject::process_set_3G(QString apn,QString pin ,QString phone,QString user,QString password)
{
    log4cpp->debug("process_set_3G");
    Q_UNUSED(phone);
    if (apn=="init") {process_init_3G();return;}
    QProcess *qProc = new QProcess(this);
    QString b;


    QFile in("wvdial.cfg");
    QFile out("/etc/wvdial.conf");
    if (in.open( QFile::ReadOnly))
    {
        QTextStream stream_in(&in);
        QTextStream stream_out(&out);
        QString s=stream_in.readAll();
        s.replace("root",user);
        s.replace("time2bii",password);
        s.replace("websfr",apn);
        s.replace("5675",pin);

        out.open( QFile::WriteOnly);
        stream_out << s;
        stream_out.flush();
        in.close();
        out.close();
    }

    b= QString("ip route del");
    qProc->start(b);
    qProc->waitForReadyRead();
    qProc->readAll();
    b= QString("/etc/init.d/networking restart");
    qProc->start(b);
    QTest::qWait(1000);
    while ( qProc->state()==QProcess::Running) QTest::qWait(100);
    qProc->close();
    qProc->deleteLater();
}



/**
 * @brief Process_qobject::process_init_3G
 */
void Process_qobject::process_init_3G(void)
{
    log4cpp->debug("process_init_3G");
    QString apn,pin, phone,user,password;

    QFile in("/etc/wvdial.conf");
    if (in.open( QFile::ReadOnly))
    {
        QTextStream stream_in(&in);
        QString s;
        do{
            s = stream_in.readLine();
            if (s.contains("Username=")) user=s.section('=',1,1);
            if (s.contains("Password=")) password=s.section('=',1,1);
            if (s.contains("Phone=")) phone=s.section('=',1,1);
            if (s.contains("Init1=")) pin=s.section('"',1,1);
            if (s.contains("Init3="))
            {
                s=s.section(',',2,2);
                apn=s.section('"',1,1);
            }

        } while (!s.isNull());

        in.close();
    }

    signal_ihm_init_3G(apn,pin,phone,user,password);

}

/**
 * @brief Process_qobject::process_set_SSH
 * - appel process_init_SSH()
 * @param start_stop
 */
void Process_qobject::process_set_SSH(QString start_stop)
{
    log4cpp->debug("process_set_SSH " + start_stop.toStdString());
    QProcess *qProc = new QProcess(this);
    QString b;
    if (start_stop=="init") {process_init_SSH();return;}
    if (start_stop=="start")
        b= QString("/etc/init.d/sshd start");
    else
        b= QString("/etc/init.d/sshd stop");
    qProc->execute("/bin/bash", { "-c", b});
    if(!qProc->waitForReadyRead())
        log4cpp->warn(QString(qProc->readAllStandardError()).toStdString());
    qProc->waitForFinished(500);
    if(qProc->state() != QProcess::NotRunning)
        qProc->kill();
    qProc->deleteLater();
}

/**
 * @brief Process_qobject::process_init_SSH
 * - appel signal_ihm_init_SSH()
 */
void Process_qobject::process_init_SSH(void)
{
    log4cpp->debug("process_init_SSH");
    QProcess *qProc = new QProcess(this);
    QString b;
    b= QString("/etc/init.d/sshd status");
    qProc->start(b);
    QTest::qWait(500);
    while ( qProc->state()==QProcess::Running) QTest::qWait(100);
    QByteArray result = qProc->readAll();
    log4cpp->debug("process_init_SSH " + result);
    qProc->close();
    qProc->deleteLater();
    signal_ihm_init_SSH("");
}

/**
 * @brief Process_qobject::process_set_SwUpdate
 * - appel process_init_SwUpdate()
 * @param start_stop
 */
void Process_qobject::process_set_SwUpdate(QString start_stop)
{
    log4cpp->debug("process_set_SwUpdate " + start_stop.toStdString());
    QProcess *qProc = new QProcess(this);
    QString b;
    if (start_stop=="init") {process_init_SwUpdate();return;}
    if (start_stop=="start")
        b= QString("/etc/init.d/swupdate start");
    else
        b= QString("/etc/init.d/swupdate stop");
    qProc->execute("/bin/bash", { "-c", b});
    if(!qProc->waitForReadyRead())
        log4cpp->warn(QString(qProc->readAllStandardError()).toStdString());
    qProc->waitForFinished(500);
    if(qProc->state() != QProcess::NotRunning)
        qProc->kill();
    qProc->deleteLater();
}

/**
 * @brief Process_qobject::process_init_SwUpdate
 * - appel signal_ihm_init_SwUpdate()
 */
void Process_qobject::process_init_SwUpdate(void)
{
    log4cpp->debug("process_init_SwUpdate");
    QProcess *qProc = new QProcess(this);
    QString b;
    b= QString("/etc/init.d/swupdate status");
    qProc->start(b);
    QTest::qWait(500);
    while ( qProc->state()==QProcess::Running) QTest::qWait(100);
    QByteArray result = qProc->readAll();
    log4cpp->debug("process_init_SwUpdate " + result);
    qProc->close();
    qProc->deleteLater();
    signal_ihm_init_SwUpdate("");
}

/**
 * @brief Process_qobject::process_browser
 * - utilisé par BrowserController::parseXml_browser()
 * - appel signal_ihm_browser() pour affichage IHM
 * @param url
 * @param proxy
 * @param audio
 * @param tempo
 */
void Process_qobject::process_browser(QString url,QString proxy,QString audio,QString tempo)
{
    log4cpp->debug("process_browser");
    if (!audio.isEmpty())
    {
        audio.prepend("/var/x7/contents/wav/");
        QFile faudio(audio);
        if (!faudio.exists()) {audio.clear();audio.append("/var/x7/contents/wav/bell.wav");}
    }

    signal_ihm_browser(url,proxy,audio,tempo);

}

/**
 * @brief Process_qobject::video
 * @param nom
 * @param tempo
 */
void Process_qobject::video(QString nom,QString tempo)
{
    log4cpp->debug("video");
    if (!nom.isEmpty())
    {
        nom.prepend("/var/x7/contents/mp4/");
        QFile fvideo(nom);
        if (fvideo.exists()) {nom.prepend("file:");}
        else {nom.clear();nom.append("file:/var/x7/contents/mp4/gfi.mp4");}
    }

    signal_video(nom,tempo);

}

/**
 * @brief Process_qobject::display_message
 */
void Process_qobject::display_message(QString titre1,QString texte1,QString titre2,QString texte2,QString texte2_1,QString titre3,QString texte3,QString texte3_1,QString c1,QString c2,QString c3,QString c4,QString badge,QString audio,QString tempo)
{
    log4cpp->debug("display_message");
    if (!audio.isEmpty())
    {
        audio.prepend("/var/x7/contents/wav/");
        QFile faudio(audio);
        if (!faudio.exists()) {audio.clear();audio.append("/var/x7/contents/wav/bell.wav");}
    }

    signal_display_message(titre1,texte1,titre2,texte2,texte2_1,titre3,texte3,texte3_1,c1,c2,c3,c4,badge,audio,tempo);

}

/**
 * @brief Process_qobject::set_touches_fonction
 * - Enregristre les touches de fonctions dans database_setup.db
 * @param libelle
 * @param activite
 * @param url
 * @param proxy
 * @param tempo
 */
void Process_qobject::set_touches_fonction(QString libelle,QString activite,QString url,QString proxy,QString tempo)
{
    log4cpp->debug("set_touches_fonction");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","touchesfonction_setup");
    db.setDatabaseName("/var/x7/etc/database_setup.db");

    if (db.open())
    {
        log4cpp->debug("Acces table touchesfonction");
        QSqlQuery query(db);
        if (!libelle.compare("DELETE") && !activite.compare("DELETE") && !url.compare("DELETE") && !proxy.compare("DELETE") && !tempo.compare("DELETE"))
        {
            query.exec(QString("DELETE from touchesfonction"));
            query.finish();
            db.commit();
            db.close();
        }
        else if (!libelle.compare("FIN") && activite.isEmpty() && url.isEmpty() && proxy.isEmpty() && tempo.isEmpty())
        {
            signal_window_set_touchesfonction();
            db.close();
        }
        else
        {
            query.prepare("INSERT INTO touchesfonction (libelle, activite, url, proxy, tempo)" "VALUES (:libelle, :activite, :url, :proxy, :tempo);");
            query.bindValue(":libelle", libelle);
            query.bindValue(":activite", activite);
            query.bindValue(":url", url);
            query.bindValue(":proxy", proxy);
            query.bindValue(":tempo", tempo);
            query.exec();
            query.finish();
            db.commit();
            db.close();
        }
    }


}

/**
 * @brief Process_qobject::qml_message
 * - Méthode pour activer reboot ou restart
 * @param s
 */
void Process_qobject::qml_message(QString s)
{
    log4cpp->debug("qml_message");
    log4cpp->info(s.toStdString());
    QString a;
    QProcess *qProc = new QProcess(this);
    if(!s.compare("reboot") )
    {
        a.append("reboot");
        qProc->startDetached(a);
    }
    else
        if(!s.compare("restart")) {
            a.append("./restart_shell");
            qProc->startDetached(a);
            //signal_window_restart_app();
        }
   // qProc->startDetached(a);
}



/* PRIVATE Function */

/**
 * @brief Process_qobject::delayBackLight
 * - Méthode pour activer le rétroéclairage
 * - appelé dans les fonctions de test de time2bii_tests
 */
void Process_qobject::delayBackLight()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

/* END PRIVATE Function */
/* PRIVATE SLOTS */


/**
 * @brief Process_qobject::priv_sleep_mode_control
 * - Methode gestion du rétroéclairage.
 * - mode veille est non fonctionnel.
 */
void Process_qobject::priv_sleep_mode_control(void)
{
    log4cpp->debug("priv_sleep_mode_control");
    return;
    //TODO supprimer cette methode
    QString a;

    if (sleep_mode_etat==1)
    {
        X7CarteIO::getInstance()->setBrightness(backlight_N2.toInt());
        //a=QString("./backlight_pwm %1").arg(backlight_N2);
        //int x=((100-backlight_N2.toInt())*255)/100;
        sleep_mode_etat=2;
        timer_sleep_mode_control->start((delai_OFF.toInt())*1000);
    }
    else if (sleep_mode_etat==2)
    {
        /* modif version 1.6
         //if (!sleep_mode.isEmpty() && sleep_mode.compare("0"))
         if (!sleep_mode.isEmpty() && sleep_mode.compare("0"))
             {
             emit badge_sleep_mode();
             a=QString("./sleep_mode");
             b=QString("i2cset -y 1 0x60 0x02 0xFF");
//             a=QString("i2cset -y 1 0x20 0x0F 0x88");
            }
         else
             {
             a=QString("./backlight_pwm 0");
             b=QString("i2cset -y 1 0x60 0x02 0xFF");
             //            a=QString("i2cset -y 1 0x20 0x0F 0x88");
             }
*/
        X7CarteIO::getInstance()->setBrightness(5);
        //a=QString("./backlight_pwm 0");
        sleep_mode_etat=3;
    }

   //if (!a.isEmpty())
   //{
   //     QProcess *qProc = new QProcess(this);
   //     qProc->startDetached(a);
   // }
}

/**
 * @brief Process_qobject::priv_refresh_batterie
 * - Methode qui teste la présence de la batterie et applique la stratégie de sauvegarde d'énergie.
 * - Appel priv_sleep_mode_control
 */
/*
 Réduction de la conso sur batterie :
 A REVOIR ENTIEREMENT
*/
void Process_qobject::priv_refresh_batterie(void)
{
    log4cpp->debug("priv_refresh_batterie");
    X7CarteIO::getInstance()->getPowerLevel(2);
    return;
    //  while (port->waitForReadyRead(100));
    batterie=0;
    log4cpp->debug("priv_refresh_batterie " + X7CarteIO::getInstance()->getPowerLevelResult().toStdString());
    if (!sleep_mode.isEmpty() && !sleep_mode.compare("1"))
    {
        log4cpp->debug("priv_refresh_batterie 1");
        batterie=1;
    }else
    {
        log4cpp->debug("priv_refresh_batterie 2");
       // message="ceci est un message ";
       // received_msg.clear();
       // transmitMsg(port);
       // receiveMsg(port);
       // port->close();
       // if (!received_msg.compare(message))
            batterie=1;
    }
    log4cpp->debug("priv_refresh_batterie 3");
    QString a;
    if (batterie && (delai_OFF.toInt()))
    {
        log4cpp->debug("priv_refresh_batterie 4");
        if (sleep_mode_etat<=0)
        {
            log4cpp->debug("priv_refresh_batterie 5");
            X7CarteIO::getInstance()->setBrightness(backlight_N1.toInt());
//            a=QString("./backlight_pwm %1").arg(backlight_N1);
            timer_sleep_mode_control->start((delai_N2.toInt())*1000);
            sleep_mode_etat=1;
        }
    }
    else /*ecran ok backlight max, alim USB */
    {
        log4cpp->debug("priv_refresh_batterie 6");
        timer_sleep_mode_control->stop();
        if (sleep_mode_etat !=0)
        {
            log4cpp->debug("priv_refresh_batterie 7");
            //            a=QString("i2cset -y 1 0x20 0x0F 0x00");
            X7CarteIO::getInstance()->setBrightness(25);
            //a=QString("./backlight_pwm 100");
            sleep_mode_etat=0;
        }
    }
//    if (!a.isEmpty())
//    {
//        QProcess *qProc = new QProcess(this);
//        qProc->startDetached(a);
//    }
    log4cpp->debug("priv_refresh_batterie 8");
    timer_batterie->start(3000);
}

/**
 * @brief Process_qobject::priv_process_test_interface
 * - utilisé dans priv_process_test_ip() (NOT USED) pour rechercher les interfaces réseaux
 * */
QString Process_qobject::priv_process_test_interface()
{
    log4cpp->debug("priv_process_test_interface");
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
    return(interface_name);
}

/**
 * @brief (NOT USED) Process_qobject::priv_process_test_ip
 * - recherche la premiere interface réseau valable et retourne son adresse Ipv4
 * @return bool : L'IP existe
 */
bool Process_qobject::priv_process_test_ip()
{
    log4cpp->debug("priv_process_test_ip");
    //TODO BG Factoriser: cette fonction existe aussi dans supervision
    // test adresse ip OK
    QList<QNetworkInterface> interfaces ;
    QHostAddress ip;
    QString interface_name=priv_process_test_interface();
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
                    ip = entry.ip();
                }
            }
        }
    }
    return(! ip.isNull());
}

/**
 * @brief Process_qobject::priv_test_lecteur
 * - On récupère le nom du lecteur pour tester la communication
 * @param
 * @param
 */
void Process_qobject::priv_test_lecteur(int a)
{
    log4cpp->debug("priv_test_lecteur");
    emit signal_ihm_retour_test(2,a);
    delayBackLight();
    int size = X7GestLecteur::getInstance()->getVersionLecteur().size();
    if(size>0)
        emit signal_ihm_retour_test(1,a);
    else
        emit signal_ihm_retour_test(0,a);
}

/**
 * @brief Process_qobject::priv_test_relay
 * @param a
 */
void Process_qobject::priv_test_relay(int a)
{
    log4cpp->debug("priv_test_relay");
    emit signal_ihm_retour_test(2,a);
    delayBackLight();
    X7CarteIO::getInstance()->setIOState(1,2,2);
    delayBackLight();
    delayBackLight();
    emit signal_ihm_retour_test(1,a);
}


/**
 * @brief Process_qobject::priv_set_date_heure
 * - Methode pour metre à jour la date et l'heure système.
 * - Utilisé dans les WS et l'ecran de configuration
 * @param an
 * @param mois
 * @param jour
 * @param heure
 * @param minute
 * @param seconde
 */
void Process_qobject::priv_set_date_heure(QString an,QString mois,QString jour,QString heure,QString minute,QString seconde)
{
    log4cpp->debug("priv_set_date_heure");
    //TODO BG reduir le nombre de paramètre.
    QString a= QString("date %1%2%3%4%5.%6").arg(mois).arg(jour).arg(heure).arg(minute).arg(an).arg(seconde);
    QProcess *qProc = new QProcess(this);
    qProc->start(a);
    qProc->waitForReadyRead();
    QString tmp = qProc->readAll();
    qProc->close();
    qProc->deleteLater();
    a= QString("hwclock -w -u");
    QProcess *qProc1 = new QProcess(this);
    qProc1->start(a);
    qProc1->waitForReadyRead();
    tmp = qProc1->readAll();
    qProc1->close();
    qProc1->deleteLater();

}


/**
 * @brief Process_qobject::priv_set_timezone_langue
 * - Methode pour metre à jour la timezone.
 * @param mode
 * @param timezone
 * @param langue
 */
void Process_qobject::priv_set_timezone_langue(int mode,QString timezone,QString langue)
{
    log4cpp->debug("priv_set_timezone_langue");
    QProcess *qProc = new QProcess(this);
    QString a;

    QFile in("../etc/timezone_langue.cfg");
    QFile out("/etc/profile.d/timezone_langue.sh");
    if (in.open( QFile::ReadOnly))
    {
        QTextStream stream_in(&in);
        QTextStream stream_out(&out);
        QString s=stream_in.readAll();
        a=QString("TZ=%1").arg('"'+timezone+'"');
        s.replace("TZ=",a);
        a=QString("LC_TIME=%1").arg(langue);
        s.replace("LC_TIME=",a);
        out.open( QFile::WriteOnly);
        stream_out << s;
        stream_out.flush();
        in.close();
        out.close();
    }

    if (mode) sleep(5);
    else QTest::qWait(5000);

    qProc->startDetached("./restart_shell");
}

/**
 * @brief Process_qobject::priv_ts_calibrate_init
 * - Supression configuration de l'ecran tactile
 */
void Process_qobject::priv_ts_calibrate_init(void)
{
    QProcess *qProc = new QProcess(this);
    qProc->start("/bin/sh", { "-c" , " > /etc/pointercal" });
    qProc->waitForReadyRead();
    log4cpp->debug(QString("priv_ts_calibrate_init : %1").arg(QString(qProc->readAll())).toStdString());
    qProc->waitForFinished();
    qProc->close();
    qProc->deleteLater();
    //QWSServer::instance()->closeMouse();
    //QWSServer::instance()->openMouse();
}

/**
 * @brief Process_qobject::priv_ts_calibrate
 * - Lancement de l'application de calibration de la dalle tactile.
 */
void Process_qobject::priv_ts_calibrate(void)
{
    log4cpp->debug("priv_ts_calibrate");
    QString a= QString("./ts_calibrate_shell");
    QProcess *qProc = new QProcess(this);
    qProc->startDetached(a);
    QString b= QString("killall time2bii");
    QProcess *qProc1 = new QProcess(this);
    qProc1->startDetached(b);

}

// Serial functions
/**
 * @brief Process_qobject::priv_transmitMsg
 * @param serial_port
 */
void Process_qobject::priv_transmitMsg(QSerialPort *serial_port)
{
    log4cpp->debug("priv_transmitMsg");
    serial_port->open(QIODevice::ReadWrite);
    serial_port->clear();
    serial_port->write(message.toLatin1(),
                       message.length());

}
/**
 * @brief Process_qobject::priv_receiveMsg
 * @param serial_port
 */
void Process_qobject::priv_receiveMsg(QSerialPort *serial_port)
{
    log4cpp->debug("priv_receiveMsg");
    char buff[1024];
    int numBytes;

    while (serial_port->waitForReadyRead(100));
    numBytes = serial_port->bytesAvailable();

    if(numBytes > 1024) numBytes = 1024;

    int i =serial_port->read(buff, numBytes);
    buff[i] = '\0';
    QString msg = buff;

    received_msg.append(msg);
    serial_port->clear();
}

// NOT USED
/**
 * @brief (NOT USED) Process_qobject::priv_init_process
 */
void Process_qobject::priv_init_process(void)
{
    //log4cpp->debug("init_process");
    //set_nom("init_nom"); //initialisation du fichier /etc/network/interfaces si besoin
    //init_ntp();
    //();
}

/**
 * @brief (NOT USED) Process_qobject::priv_refresh_ntp
 * - Mise à jour de l'heure sytème par serveur ntp
 * - appel priv_process_test_ip()
 */
void Process_qobject::priv_refresh_ntp(void)
{
    log4cpp->debug("priv_refresh_ntp");
    int x;
    QChar type;
    QString b;

    if (!serveur_ntp.isEmpty() && !serveur_ntp.contains(' ') )
    {
        //log4cpp->debug("ntp not empty");
        if (priv_process_test_ip())
        {
            //log4cpp->debug("ip not empty");
            if (!cadence_ntp.isEmpty() && !cadence_ntp.contains(' '))
            {
                //log4cpp->debug("cadence not empty");
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
                log4cpp->debug(a.toStdString());
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
 * @brief (NOT USED) Process_qobject::priv_refresh_reboot
 * - Methode qui si le fichier 'reboot' existe reboot le sytème.
 * Et si le fichier 'restart' existe restart juste l'application via le script 'restart_shell'
 */
void Process_qobject::priv_refresh_reboot(void)
{
    log4cpp->debug("refresh_reboot");
    return;
    //TODO supprimer cette methode
    QFile file("reboot");
    QFile file1("restart");
    QString a;
    int i=0;
    if (file.exists())  {file.remove();i=1;a.append("reboot");}  //reboot complet
    if (file1.exists())  {file1.remove();i=1;a.append("art_shell");}  //restart application
    if (i)
    {
        QProcess *qProc = new QProcess(this);
        qProc->startDetached(a);
    }
    timer_reboot->start(1500);// reessai toutes les 1500 millisecondes
}

/**
 * @brief (NOT USED) Process_qobject::priv_refresh_rebootauto
 * - Methode qui si le fichier 'rebootauto' existe rebot le système a l'heure indiquée dans le fichier Ex : 03:30
 */
void Process_qobject::priv_refresh_rebootauto(void)
{
    log4cpp->debug("refresh_rebootauto");
    return;
    //TODO supprimer cette methode
    QFile file("../etc/rebootauto");
    int i=0;
    bool ok;
    if (file.exists()) // reboot total
    {
        if(file.open(QFile::ReadOnly))
        {
            QTime t=QTime::currentTime();
            QTextStream stream_in(&file);
            QString s = stream_in.readLine();
            //                     log4cpp->debug("rebootauto:"+ s);
            if (s.contains(":"))
            {
                int h=s.section(':',0,0).toInt(&ok,10);
                int m=s.section(':',1,1).toInt(&ok,10);;
                if (t.hour()==h && t.minute()==m && t.second() >=0 && t.second() <=5) i=1;
                else timer_rebootauto->start(1000);// reessai toutes les  secondes
            }
            else
            {
                QString a=QString("cat /proc/meminfo");
                int k=0;
                QProcess *qProc = new QProcess(this);
                qProc->start(a);
                qProc->waitForReadyRead();
                QString tmp = qProc->readAll();
                tmp.replace(QString("kB"),QString(":"));
                tmp.replace(QString(" "),QString(""));
                qProc->close();
                qProc->deleteLater();
                if (s.length() >0) k=s.toInt(&ok,10);
                if (k<20000) k=20000;
                if (k> 100000) k=100000;
                QString freemem=tmp.section(':',3,3);
                //                         log4cpp->debug("Freemem"+freemem + "int:" + freemem.toInt());
                //                         log4cpp->debug("k:%d", k);
                if (freemem.toInt(&ok,10) < k) i=1;
                else timer_rebootauto->start(60000);// reessai toutes les 60 secondes
            }
            file.close();
        }
    }
    else
    {
        QString a=QString("cat /proc/meminfo");
        QProcess *qProc = new QProcess(this);
        qProc->start(a);
        qProc->waitForReadyRead();
        QString tmp = qProc->readAll();
        tmp.replace(QString("kB"),QString(":"));
        tmp.replace(QString(" "),QString(""));
        qProc->close();
        qProc->deleteLater();
        QString freemem=tmp.section(':',3,3);
        //log4cpp->debug(freemem << "int : "<< freemem.toInt());
        if (freemem.toInt(&ok,10) < 20000) i=1;
        else timer_rebootauto->start(60000);// reessai toutes les 60 secondes
    }

    if (i)
    {
        QFile file1("../log/rebootauto_log");
        QString b=QString("cat /proc/meminfo");
        QProcess *qProc = new QProcess(this);
        qProc->start(b);
        qProc->waitForReadyRead();
        QString tmp = qProc->readAll();
        if (file1.open(QFile::ReadWrite | QIODevice::Text))
        {
            file1.write(tmp.toLatin1().data());
            file1.flush();
            file1.close();
        }
        sleep(2);
        qProc->startDetached("reboot");
    }

}

/**
 * @brief (NOT USED) Process_qobject::priv_refresh_restartauto
 * - Methode qui si le fichier 'restartauto' existe restart l'application a l'heure indiquée dans le fichier Ex : 03:30
 */
void Process_qobject::priv_refresh_restartauto(void)
{
    log4cpp->debug("priv_refresh_restartauto");
    return;
    //TODO supprimer cette methode
    QFile file("../etc/restartauto");
    int i=0;
    bool ok;
    if (file.exists()) // restart application
    {
        if(file.open(QFile::ReadOnly))
        {
            QTime t=QTime::currentTime();
            QTextStream stream_in(&file);
            QString s = stream_in.readLine();
            //                     log4cpp->debug("restartauto: "+ s.toStdStrng());
            if (s.contains(":"))
            {
                //                         log4cpp->debug(s);
                int h=s.section(':',0,0).toInt(&ok,10);
                int m=s.section(':',1,1).toInt(&ok,10);;
                if (t.hour()==h && t.minute()==m && t.second() >=0 && t.second() <=5) i=1;
                else timer_restartauto->start(1000);// reessai toutes les  secondes
            }
            else
            {
                QString a=QString("cat /proc/meminfo");
                int k=0;
                QProcess *qProc = new QProcess(this);
                qProc->start(a);
                qProc->waitForReadyRead();
                QString tmp = qProc->readAll();
                tmp.replace(QString("kB"),QString(":"));
                tmp.replace(QString(" "),QString(""));
                qProc->close();
                qProc->deleteLater();
                if (s.length() >0) k=s.toInt(&ok,10);
                if (k<20000) k=20000;
                if (k> 100000) k=100000;
                QString freemem=tmp.section(':',3,3);
                //                         log4cpp->debug("Freemem=" +freemem + " int : " + freemem.toInt());
                //                         log4cpp->debug("k:%d", k);
                if (freemem.toInt(&ok,10) < k) i=1;
                else timer_restartauto->start(60000);// reessai toutes les 60 secondes
            }
            file.close();
        }
    }
    else timer_restartauto->start(60000);// reessai toutes les 60 secondes

    if (i)
    {
        file.setFileName("../log/restartauto_log");
        QString b=QString("cat /proc/meminfo");
        QProcess *qProc = new QProcess(this);
        qProc->start(b);
        qProc->waitForReadyRead();
        QString tmp = qProc->readAll();
        if (file.open(QFile::ReadWrite | QIODevice::Text))
        {
            file.write(tmp.toLatin1().data());
            file.flush();
            file.close();
        }
        qProc->startDetached("art_shell");
    }

}

/**
 * @brief (NOT USED) Process_qobject::priv_refresh_usbkey
 * - Gestion d'une clé usb (montage démontage) et copie de la BDD de badgeage dessus.
 * - le fichier de base de donnée est copié sour la forme nomTime2bii_jour_heure.db
 */
void Process_qobject::priv_refresh_usbkey(void)
{
    log4cpp->debug("refresh_usbkey");
    if (Supervision_qobject::getInstance()->http_server.isEmpty())
    {
        if (usbkey_mounted==0)
        {
            QFile file ("/dev/sda1");
            if (file.exists())
            {
                QString a= QString("mount /media/usbkey");
                QProcess *qProc = new QProcess(this);
                qProc->start(a);
                qProc->waitForReadyRead();
                QString tmp = qProc->readAll();
                if (tmp.isEmpty())
                {
                    usbkey_mounted=1;
                    emit signal_ihm_usbkey_status(1);
                }
                else
                {
                    a= QString("umount /media/usbkey");
                    qProc->start(a);
                    qProc->waitForReadyRead();
                    usbkey_mounted=0;
                    emit signal_ihm_usbkey_status(0);
                }
                qProc->close();
                qProc->deleteLater();
            }
        }
        else if (usbkey_mounted==1)
        {
            QString nom=supervision_thread.supervision.time2bii_name;
            nom=nom.simplified();
            QString j=QDate::currentDate().toString(Qt::ISODate);
            QString h=QTime::currentTime().toString(Qt::ISODate);
            h.replace(":","-");

            QString a=QString("/media/usbkey/%1_%2_%3.db").arg(nom).arg(j).arg(h);
            bool x=QFile::copy("/var/x7/contents/database_badge.db",a);
            if(x)
            {
                a= QString("umount /media/usbkey");
                QProcess *qProc = new QProcess(this);
                qProc->start(a);
                qProc->waitForReadyRead();
                qProc->readAll();
                if (process_badge_db.open())
                {
                    QSqlQuery query(process_badge_db);
                    query.exec(QString("DELETE FROM badge"));
                    query.exec(QString("UPDATE SQLITE_SEQUENCE SET SEQ=0 WHERE NAME='badge'"));
                    query.exec(QString("VACUUM"));
                    process_badge_db.close();
                }
                usbkey_mounted=2;
                emit signal_ihm_usbkey_status(2);
                qProc->close();
                qProc->deleteLater();
            }
        }
        else if (usbkey_mounted==2)
        {
            QFile file ("/dev/sda1");
            if (!file.exists())
            {
                emit signal_ihm_usbkey_status(0);
                usbkey_mounted=0;
            }
        }
        timer_usbkey->start(1111);
    }
}

/**
 * @brief (NOT USED) Process_qobject::priv_audio_set_volume
 * - Permet de changer le volume de l'application
 * @param a si 0 muet si 1 incrément de 1 entre les valeurs 110 à 126.♠
 */
void Process_qobject::priv_audio_set_volume(QString a)
{
        return;
}

//END NOT USED
/* END PRIVATE SLOTS */

// QTHREAD
/**
 * @brief (NOT USED) Process_qthread::init
 * - init badge_thread et supervision_thread
 */
void Process_qthread::proc_qthread_init(void)
{
    /*badge_thread.start();
supervision_thread.start();
*/}

/**
 * @brief Process_qthread::run
 * - lance Process_qobject::process_init()
 */
void Process_qthread::run()
{

    //proc_qthread_init(); //init badge_thread et supervision_thread (NOT USED)
    process.process_init();
    exec();
}





