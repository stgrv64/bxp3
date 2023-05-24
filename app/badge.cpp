/**
 * @file
 * @brief Badge_qobject and Badge_qthread class definition
*******************************************************************/

#include <badge.h>
#include <../BadgeLib/badgelib.h>
#include <process.h>
#include <supervision.h>
//#include <QDebug>
#include "T2bEvent.h"

using namespace std;

log4cpp::Category *badgelog;
log4cpp::Category *badgeThreadlog;

/**
 * @brief constructor Badge_qobject::Badge_qobject
 */
Badge_qobject::Badge_qobject(void)
{
    //log4cpp::Category & mylog_ = log4cpp::Category::getInstance("badge");
    badgelog = &log4cpp::Category::getInstance("Badge_qobject");
    //mylog = &mylog_;
    badgelog->debug("Badge_qobject::init0");
}

/**
 * @brief Badge_qobject::init
 */
void Badge_qobject::init(void)
{
    timer_init =new QTimer();
    timer_init->setSingleShot(true);
    connect(timer_init,SIGNAL(timeout()),this,SLOT(init0()));
    //timer_init->start(1);
}

void Badge_qobject::badge_clear_slot()
{
    badgeS.clear();
    badgeS_morpho.clear();
}

void Badge_qobject::sleep_mode_slot()
{
    morpho_run=0;
    switch (lecteur.toInt())
    {
    case STID_LEGIC:
        timer_timeout_stid_legic->start(1000);
        break;

    case INSIDE:
        timer_timeout_inside->start(1000);
        break;

    case MORPHOouWIEGAND:
        morpho_run=1;
        timer_timeout_morpho->start(1000);
        if (option.toInt() >=0)
        {
            timer_timeout_badge->stop();
            timer_attente_badge->start(2000);
        }
        break;

    case MORPHOetWIEGAND:
        morpho_run=1;
        timer_timeout_morpho->start(1000);
        timer_timeout_badge_morpho->stop();
        timer_attente_badge_morpho->start(2000);
        break;

    case ISO2:
    case WIEGAND:  // lecteur data_clock iso2 ou wiegand
        timer_timeout_badge->stop();
        timer_attente_badge->start(2000);
        break;
    }
    sleep_mode=1;
}


/**
 * @brief Badge_qobject::init0
 */
void Badge_qobject::init0(void)         
{
    UC				i_uc_Settings;

    UC				o_uc_FingerNb;
    UL				o_ul_MaxRecord;
    UL				o_ul_CurrentRecord;
    UC				o_uc_EncrytedDB;
    UL				io_ul_AddFieldNb;
    T_FIELD         o_x_AddField[3];
    UC				o_uc_NormalizedPK_Type;	//can be NULL
    UC				o_uc_ILV_Status;
    UL				o_ul_EmbeddedError;
    badgelog->debug("init0");
    int erreur=0;
    int node=0;
    badgeS.reserve(100);
    badgeS.clear();
    QString y(50);
    QString module_name(11);
    sleep_mode=0;
    //QFile f("badge_debug");
    //if (f.exists()) badge_debug=1; else badge_debug=0;
    morpho_run=0;
    morpho.mutex.unlock();
    cmd_index=0;

    /* formats "standard", differencies par nombre de bits */
    /*bits|  parite 1 |  parite 2 |  data 1   |  Data 2    |  Data 3     */
    //    Wieg25bitsNCS = { 25, 25,17,24,0,   0, 0, 0,0, 17,24,0, 3,   2,16,0, 5,  0, 0,0, 0};
    //    Wieg26bits       = { 26,  1, 2,13,0,  26,14,25,1,  2, 9,0, 3,  10,25,0, 5,  0, 0,0, 0};
    //    Wieg30bitsAccor  = { 30,  1, 2,16,0,  30,17,29,1,  2, 9,0, 3,  10,13,0, 2, 14,29,0, 5};
    //    Wieg34bits       = { 34, 34, 1,33,0,   0, 0, 0,0,  2,17,0, 5,  18,33,0, 5,  0, 0,0, 0};
    //    Wieg35bHIDCorp   = { 35,  1, 2,35,1,   0, 0, 0,0,  3,14,0, 4,  15,34,0, 7,  0, 0,0, 0};
    //    Wieg36bitsclock  = { 36,  0, 0, 0,0,   0, 0, 0,0,  1,32,2, 9,   0, 0,0, 0,  0, 0,0, 0};
    //    Wieg36btechtalk6 = { 36, 13,19,36,1,   0, 0, 0,0, 19,20,0, 1,  21,36,0, 5,  0, 0,0, 0};
    //    Wieg37bitsnosite = { 37,  1, 2,19,0,  37,19,36,1,  2,36,0,11,   0, 0,0, 0,  0, 0,0, 0};
    //    Wieg44bitshexa   = { 44,  0, 0, 0,0,   0, 0, 0,0,  1,40,3,10,   0, 0,0, 0,  0, 0,0, 0};
    //    Wieg68bitshexa   = { 68,  0, 0, 0,0,   0, 0, 0,0,  1,64,3,16,   0, 0,0, 0,  0, 0,0, 0};
    //    Wieg112bitshexa  = {112,  0, 0, 0,0,   0, 0, 0,0, 1,108,2,27,   0, 0,0, 0,  0, 0,0, 0};

    /* Valeur de lect->option sur lecteur Wiegand (Readerx = 8) */


    config_db = QSqlDatabase::addDatabase("QSQLITE","badge_setup");
    config_db.setDatabaseName("/var/x7/etc/database_setup.db");

    timer_attente_badge =new QTimer();
    timer_timeout_badge =new QTimer();
    timer_timeout_stid_legic =new QTimer();
    timer_timeout_inside =new QTimer();

    timer_timeout_badge_morpho =new QTimer();
    timer_attente_badge_morpho =new QTimer();
    timer_timeout_morpho =new QTimer();


    lecteur.clear();
    option.clear();
    masque.clear();
    format.clear();

    QString a=QString("rmmod data_clock");
    QString b=QString("rm /dev/data_clock");
    QString c=QString("rmmod wiegand");
    QString d=QString("rm /dev/wiegand");
    QProcess *qProc = new QProcess(this);
    qProc->start(a);
    qProc->waitForFinished();
    qProc->start(b);
    qProc->waitForFinished();

    qProc->start(c);
    qProc->waitForFinished();
    qProc->start(d);
    qProc->waitForFinished();
    qProc->deleteLater();


    QFile file ("badge_config");
    if (file.exists())
    {
        if (file.open(QFile::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QString line = in.readLine();
            lecteur=line.section(" ",0,0);
            option=line.section(" ",1,1);
            masque=line.section(" ",2,2);
            format=line.section(" ",3,3);
            file.close();
        }

        if (config_db.open())
        {
            badgelog->info("Ecriture table badge");
            QSqlQuery query(config_db);
            query.exec(QString("DELETE from badge"));
            query.clear();
            query.finish();
            config_db.commit();
            query.prepare("INSERT INTO badge (lecteur, option, masque, format)" "VALUES (:lecteur, :option, :masque, :format);");
            query.bindValue(":lecteur", lecteur);
            query.bindValue(":option", option);
            query.bindValue(":masque", masque);
            query.bindValue(":format", format);
            query.exec();
            query.clear();
            query.finish();
            config_db.commit();
            config_db.close();
        }
        file.remove();
    }


    if (config_db.open())
    {
        badgelog->info("Lecture table badge badge.");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM badge",config_db);
        if (model.rowCount())
        {
            lecteur=model.record(0).value("lecteur").toString();
            option=model.record(0).value("option").toString();
            masque=model.record(0).value("masque").toString();
            format=model.record(0).value("format").toString();
        }
        model.clear();
        model.deleteLater();
        config_db.close();
    }
    if (lecteur.isEmpty()) lecteur="0";
    if (option.isEmpty()) option="0";
    badgelog->info("Configuration badge: Lecteur= %d  Option= %d", lecteur.toInt(), option.toInt());


    switch (lecteur.toInt())
    {

    case STID_LEGIC:
        port = new QSerialPort("/dev/ttyUSB0");
        port->setBaudRate(QSerialPort::Baud57600);
        port->setFlowControl(QSerialPort::HardwareControl);
        port->setParity(QSerialPort::NoParity);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);
        port->open(QIODevice::ReadWrite);
        timer_timeout_stid_legic->setSingleShot(true);
        connect(timer_timeout_stid_legic,SIGNAL(timeout()),this,SLOT(dialog_stid_legic()));
        timer_timeout_stid_legic->start(1000);
        break;

    case INSIDE:
        puce_type=0;
        mifare_init=0;
        presence_badge=0;
        port = new QSerialPort("/dev/ttyUSB0");
        port->setBaudRate(QSerialPort::Baud57600);
        port->setFlowControl(QSerialPort::HardwareControl);
        port->setParity(QSerialPort::NoParity);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);
        port->open(QIODevice::ReadWrite);
        timer_timeout_inside->setSingleShot(true);
        connect(timer_timeout_inside,SIGNAL(timeout()),this,SLOT(dialog_inside()));
        timer_timeout_inside->start(1000);
        break;

    case MORPHOouWIEGAND:
        i_uc_Settings=0;
        morpho.moveToThread(&morpho_thread); // lancement du thread lecture biometrique Morpho CBM
        morpho_thread.start();

        //             morpho.blob=malloc(500*500);
        MSO_InitCom(&morpho.i_h_Mso100Handle,(C*)"RS232",(C*)"/dev/ttyUSB0",115200);
        MSO_SetConfigValue(morpho.i_h_Mso100Handle,&o_uc_ILV_Status,0x0E10,1,&i_uc_Settings);// Position lecteur +180°
        MSO_RebootSoft(morpho.i_h_Mso100Handle,& o_uc_ILV_Status);

        // creation base danslecteur MORPHO si non déja créée
        MSO_BioDB_GetBaseConfig(morpho.i_h_Mso100Handle,0,&o_uc_FingerNb,&o_ul_MaxRecord,&o_ul_CurrentRecord,&o_uc_EncrytedDB,&io_ul_AddFieldNb,&o_x_AddField[0],&o_uc_NormalizedPK_Type,&o_uc_ILV_Status,&o_ul_EmbeddedError);
        if (o_uc_ILV_Status!=ILV_OK) QMetaObject::invokeMethod(&morpho, "MORPHO_CreateDataBase", Qt::QueuedConnection);

        disconnect(&morpho,SIGNAL(signal_morpho_badge(QString)),this,SLOT(morphoOUwiegand_badge(QString)));
        disconnect(&morpho,SIGNAL(signal_morpho_message(QString)),this,SLOT(morpho_message(QString)));
        connect(&morpho,SIGNAL(signal_morpho_badge(QString)),this,SLOT(morphoOUwiegand_badge(QString)));
        connect(&morpho,SIGNAL(signal_morpho_message(QString)),this,SLOT(morpho_message(QString)));

        ihm_morpho_enabled();
        timer_timeout_morpho->setSingleShot(true);
        connect(timer_timeout_morpho,SIGNAL(timeout()),this,SLOT(dialog_morpho()));
        morpho_run=1;
        timer_timeout_morpho->start(2000);
        if (option.toInt() >=0)
        {
            module_name="wiegand";
            badgelog->debug("Init module (morpho ou wiegand)");
            QString cde_modprobe=QString("/sbin/modprobe %1").arg(module_name);
            QProcess *qProc = new QProcess(this);

            // --- lancement 1er process : modprobe
            qProc->start(cde_modprobe);
            qProc->waitForFinished();
            badgelog->debug("Lancement '" + cde_modprobe.toStdString() + "'");
            qProc->deleteLater();

            device.setFileName("/proc/devices");
            if (!device.open(QFile::ReadWrite | QIODevice::Text))  erreur=1;
            else
            {
                while(!y.contains(module_name)) y=device.readLine();
                y=y.section(' ',0,0);
                node=y.toInt();
                device.close();

                // --- lancement 2e process : mknod
                QProcess *qProc1 = new QProcess(this);
                QString cde_mk=QString("mknod /dev/%1 c %2 0").arg(module_name).arg(node);
                qProc1->start(cde_mk);
                qProc1->waitForFinished();
                qProc1->deleteLater();

                badgelu.setFileName(QString("/dev/%1").arg(module_name));
                if (!badgelu.open(QFile::ReadWrite | QIODevice::Text)) erreur=1;
                else
                {
                    badgelu.close();
                    timer_timeout_badge->setSingleShot(true);
                    connect(timer_timeout_badge,SIGNAL(timeout()),this,SLOT(timeout_badge()));
                }

                if (!erreur)
                {
                    timer_attente_badge->setSingleShot(true);
                    connect(timer_attente_badge,SIGNAL(timeout()),this,SLOT(attente_badge()));
                    timer_attente_badge->start(500);
                }
            }
        }

        break;

    case MORPHOetWIEGAND:
        i_uc_Settings=0;

        morpho.moveToThread(&morpho_thread); // lancement du thread lecture biometrique Morpho CBM
        morpho_thread.start();
        //        morpho.blob=malloc(500*500);
        morpho_egal_badge=0;// test identifiant empreinte egal numero badge ?
        MSO_InitCom(&morpho.i_h_Mso100Handle,(C*)"RS232",(C*)"/dev/ttyUSB0",115200);
        MSO_SetConfigValue(morpho.i_h_Mso100Handle,&o_uc_ILV_Status,0x0E10,1,&i_uc_Settings);// Position lecteur +180°
        MSO_RebootSoft(morpho.i_h_Mso100Handle,& o_uc_ILV_Status);

        // creation base danslecteur MORPHO si non déja créée
        MSO_BioDB_GetBaseConfig(morpho.i_h_Mso100Handle,0,&o_uc_FingerNb,&o_ul_MaxRecord,&o_ul_CurrentRecord,&o_uc_EncrytedDB,&io_ul_AddFieldNb,&o_x_AddField[0],&o_uc_NormalizedPK_Type,&o_uc_ILV_Status,&o_ul_EmbeddedError);
        if (o_uc_ILV_Status!=ILV_OK) QMetaObject::invokeMethod(&morpho, "MORPHO_CreateDataBase", Qt::QueuedConnection);

        disconnect(&morpho,SIGNAL(signal_morpho_badge(QString)),this,SLOT(morphoETwiegand_badge(QString)));
        disconnect(&morpho,SIGNAL(signal_morpho_message(QString)),this,SLOT(morpho_message(QString)));
        connect(&morpho,SIGNAL(signal_morpho_badge(QString)),this,SLOT(morphoETwiegand_badge(QString)));
        connect(&morpho,SIGNAL(signal_morpho_message(QString)),this,SLOT(morpho_message(QString)));

        ihm_morpho_enabled();
        timer_timeout_morpho->setSingleShot(true);
        connect(timer_timeout_morpho,SIGNAL(timeout()),this,SLOT(dialog_morpho()));
        morpho_run=1;
        timer_timeout_morpho->start(2000);
        if (option.toInt() < 0) {option.setNum(0,10);}
        if (option.toInt() >= 100) {morpho_egal_badge=1;int x=option.toInt();x-=100;option.setNum(x,10);}
        if (option.toInt() >=0)
        {
            badgelog->debug("Init module (morpho et wiegand)");
            module_name="wiegand";
            QString cde_modprobe=QString("/sbin/modprobe %1").arg(module_name);

            // --- lancement 1er process : mod_probe
            QProcess *qProc = new QProcess(this);
            qProc->start(cde_modprobe);
            qProc->waitForFinished();
            badgelog->debug("Lancement de '" + cde_modprobe.toStdString() + "'");
            qProc->deleteLater();

            device.setFileName("/proc/devices");
            if (!device.open(QFile::ReadWrite | QIODevice::Text))  erreur=1;
            else
            {
                while(!y.contains(module_name)) y=device.readLine();
                y=y.section(' ',0,0);
                node=y.toInt();
                device.close();

                // --- lancement 2e process : mknod
                QProcess *qProc1 = new QProcess(this);
                QString cde_mknod=QString("mknod /dev/%1 c %2 0").arg(module_name).arg(node);
                qProc1->start(cde_mknod);
                qProc1->waitForFinished();
                qProc1->deleteLater();

                badgelu.setFileName(QString("/dev/%1").arg(module_name));
                if (!badgelu.open(QFile::ReadWrite | QIODevice::Text)) erreur=1;
                else
                {
                    badgelu.close();
                    timer_timeout_badge_morpho->stop();
                    timer_timeout_badge_morpho->setSingleShot(true);
                    connect(timer_timeout_badge_morpho,SIGNAL(timeout()),this,SLOT(timeout_badge_morpho()));
                }

                if (!erreur)
                {
                    timer_attente_badge_morpho->setSingleShot(true);
                    connect(timer_attente_badge_morpho,SIGNAL(timeout()),this,SLOT(attente_badge_morpho()));
                    timer_attente_badge_morpho->start(500);
                }
            }
        }
        break;

    case ISO2:
    case WIEGAND:  // lecteur data_clock iso2 ou wiegand
        port = new QSerialPort("/dev/ttyUSB0");
        port->setBaudRate(QSerialPort::Baud57600);
        port->setFlowControl(QSerialPort::HardwareControl);
        port->setParity(QSerialPort::NoParity);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);
        if (port->open(QIODevice::ReadWrite))
            badgelog->debug("port ouvert");
        else
            badgelog->debug("port error");
        timer_timeout_badge->setSingleShot(true);
        connect(timer_timeout_badge,SIGNAL(timeout()),this,SLOT(timeout_badge()));
        timer_timeout_badge->start(1000);

        break;

    }

    //_updateNomPre  = new QTimer();
    nomPre.clear();
    updateNomPre();
}

// correspondance entre les bages et les Nom Prenom pour affichage sur l'image du badge
void Badge_qobject::updateNomPre()
{
    badgelog->debug("updateNomPre");
    QString fc;
    QString nom("time2bii.nom");
    fc = fileChecksum(nom,QCryptographicHash::Md5);

    if(fc != nomPreMd5)
    {
        nomPreMd5 = fc;
        nomPre.clear();

        QFile fnom(nom);
        if (!fnom.open(QFile::ReadOnly))
        {
            badgelog->debug(fnom.errorString().toStdString());
        }else
        {
            while (!fnom.atEnd())
            {
                QByteArray line = fnom.readLine();
                QList<QByteArray> list = line.split(';');
                if(list.size()==3)
                {
                    QString n(list[1]);
                    QString p(list[2]);
                    n = n.simplified();
                    p = p.simplified();
                    n.append("\n");
                    n.append(p);
                    nomPre.insert(QString(list[0]),n);
                }
            }
            fnom.close();
        }
    }
    badgelog->debug("Liste nom :" + nomPre.size());
}

QString Badge_qobject::fileChecksum(const QString &fileName,QCryptographicHash::Algorithm hashAlgorithm)
{
    badgelog->debug("fileChecksum");
    QFile f(fileName);
    if (f.open(QFile::ReadOnly))
    {
        QCryptographicHash hash(hashAlgorithm);
        hash.addData(f.readAll());
        f.close();
        return QString(hash.result().toHex());
    }
    return QString();
}

void Badge_qobject::ihm_badge(QVariant badge, QVariant jour, QVariant heure, QVariant badgeEncode)
{
    badgelog->debug("ihm_badge");
    QVariant np = (QVariant)rechercheNomPre(badge.toString());
    emit ihm_badge(badge, jour, heure, badgeEncode, np);
}

/*void Badge_qobject::init_lecteur(void)
{
    int erreur=0;
    QString y(50);
    QString x(11);
    sleep_mode=0;
    morpho_run=0;
    QProcess *qProc1 = new QProcess(this);
    QString restart("./restart_shell");

    switch (lecteur.toInt())
    {
    case STID_LEGIC:
        port->close();
        timer_timeout_stid_legic->stop();
        disconnect(timer_timeout_stid_legic,SIGNAL(timeout()),this,SLOT(dialog_stid_legic()));
        break;

    case INSIDE:
        port->close();
        timer_timeout_inside->stop();
        disconnect(timer_timeout_inside,SIGNAL(timeout()),this,SLOT(dialog_inside()));
        break;

    case MORPHOouWIEGAND:
    case MORPHOetWIEGAND:
        morpho_thread.exit();
        ihm_morpho_status((QVariant) 1); //signal vers ihm lecteur morpho OK
        timer_attente_badge->stop();
        timer_timeout_badge->stop();
        timer_timeout_morpho->stop();
        disconnect(timer_timeout_morpho,SIGNAL(timeout()),this,SLOT(dialog_morpho()));
        disconnect(timer_timeout_badge,SIGNAL(timeout()),this,SLOT(timeout_badge()));
        disconnect(timer_attente_badge,SIGNAL(timeout()),this,SLOT(attente_badge()));
        break;

    case ISO2:
    case WIEGAND:  // lecteur data_clock iso2 ou wiegand
        timer_attente_badge->stop();
        timer_timeout_badge->stop();
        disconnect(timer_timeout_badge,SIGNAL(timeout()),this,SLOT(timeout_badge()));
        disconnect(timer_attente_badge,SIGNAL(timeout()),this,SLOT(attente_badge()));
        break;
    }

    config_db = QSqlDatabase::addDatabase("QSQLITE","badge_setup");
    config_db.setDatabaseName("/var/x7/etc/database_setup.db");

    lecteur.clear();
    option.clear();
    masque.clear();
    format.clear();

    QString a=QString("rmmod data_clock");
    QString b=QString("rm /dev/data_clock");
    QString c=QString("rmmod wiegand");
    QString d=QString("rm /dev/wiegand");
    QProcess *qProc = new QProcess(this);
    qProc->start(a);
    qProc->waitForFinished();
    qProc->start(b);
    qProc->waitForFinished();

    qProc->start(c);
    qProc->waitForFinished();
    qProc->start(d);
    qProc->waitForFinished();
    qProc->deleteLater();

    if (config_db.open())
    {
        badgelog->info("Lecture table badge.");
        QSqlQueryModel model;
        model.setQuery("SELECT * FROM badge",config_db);
        if (model.rowCount())
        {
            lecteur=model.record(0).value("lecteur").toString();
            option=model.record(0).value("option").toString();
            masque=model.record(0).value("masque").toString();
            format=model.record(0).value("format").toString();
        }
        model.clear();
        model.deleteLater();
        config_db.close();
    }
    if (lecteur.isEmpty()) lecteur="0";
    if (option.isEmpty()) option="0";

    badgelog->info("Configuration badge: Lecteur= %s  Option= %s", lecteur, option);

    switch (lecteur.toInt())
    {

    case STID_LEGIC:
        port = new QSerialPort("/dev/ttyUSB0");
        port->setBaudRate(QSerialPort::Baud57600);
        port->setFlowControl(QSerialPort::HardwareControl);
        port->setParity(QSerialPort::NoParity);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);
        port->open(QIODevice::ReadWrite);

        timer_timeout_stid_legic->setSingleShot(true);
        connect(timer_timeout_stid_legic,SIGNAL(timeout()),this,SLOT(dialog_stid_legic()));
        timer_timeout_stid_legic->start(1000);
        break;

    case INSIDE:
        puce_type=0;
        mifare_init=0;
        presence_badge=0;
        port = new QSerialPort("/dev/ttyUSB0");
        port->setBaudRate(QSerialPort::Baud57600);
        port->setFlowControl(QSerialPort::HardwareControl);
        port->setParity(QSerialPort::NoParity);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);
        port->open(QIODevice::ReadWrite);

        timer_timeout_inside->setSingleShot(true);
        connect(timer_timeout_inside,SIGNAL(timeout()),this,SLOT(dialog_inside()));
        timer_timeout_inside->start(1000);
        break;

    case MORPHOouWIEGAND:
    case MORPHOetWIEGAND:
        qProc1->startDetached(restart);
        break;

    case ISO2:
    case WIEGAND:  // lecteur data_clock iso2 ou wiegand
        port = new QSerialPort("/dev/ttyUSB0");
        port->setBaudRate(QSerialPort::Baud57600);
        port->setFlowControl(QSerialPort::HardwareControl);
        port->setParity(QSerialPort::NoParity);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);
        port->open(QIODevice::ReadWrite);

        if(timer_timeout_badge!=NULL){
            timer_timeout_badge->setSingleShot(true);
            connect(timer_timeout_badge,SIGNAL(timeout()),this,SLOT(timeout_badge()));
        }
        break;
    }
}*/

// PRIVATE SLOTS

void Badge_qobject::attente_badge_morpho(void)
{
    badgelog->debug("attente_badge_morpho");
    QString s;
    if (badgelu.open(QFile::ReadWrite | QIODevice::Text))
    {
        s=badgelu.readAll();
        badgelu.close();
        if (s.count()>0)
        {
            timer_attente_badge_morpho->stop();
            timer_timeout_badge_morpho->start(200);
        }
        else timer_attente_badge_morpho->start(100);
    }
}



void Badge_qobject::timeout_badge_morpho(void)
{
    badgelog->debug("timeout_badge_morpho");
    timer_attente_badge_morpho->stop();
    QString s;
    if (badgelu.open(QFile::ReadWrite | QIODevice::Text))
    {
        s.append(badgelu.readAll());
        badgelu.write("0",1);
        badgelu.close();
        badgelog->info(s.toStdString());
        // autorise lecture badge si comm ok ou database badge ok

        if (supervision_thread.supervision.http_etat >0 || supervision_thread.supervision.badge_db_etat >0 )
        {
            QString badge(30);

            if (option.toInt() >=0 ) badge=badge_wiegand(s);// lecteur wiegand

            QString j=QDate::currentDate().toString(Qt::ISODate);
            QString h=QTime::currentTime().toString(Qt::TextDate);

            if (!badge.compare("Badge inconnu"))
            {
                ihm_badge_inconnu(); //signal vers ihm erreur de bit badge
                badgeS.clear();
                badgeS_morpho.clear();
                timer_attente_badge_morpho->start(200);
            }
            else
            {
                badge=extraire_matricule(badge);
                badge=badge.toUpper();
                badgeS_morpho=badge;
                badge_encode_morpho=encode_badge(badge);
                if(badgeS.isEmpty()) ihm_badge_message((QVariant) QObject::trUtf8("Présenter Empreinte"));
            }
        }
    }
}


void Badge_qobject::attente_badge(void)
{
    badgelog->debug("attente_badge");
    if(port->bytesAvailable())
    {
        timer_attente_badge->stop();
        if (lecteur.toInt()==ISO2 ) timer_timeout_badge->start(400);
        else timer_timeout_badge->start(200);
    }
    else timer_attente_badge->start(100);
}



void Badge_qobject::timeout_badge(void)
{
    badgelog->debug("timeout_badge");
    QString s;
    if (port->bytesAvailable())
    {
        s.append(port->readAll());
        s = s.left(10);
        badgelog->debug("badgeage : " + s.toStdString());
        if (supervision_thread.supervision.http_etat >0 || supervision_thread.supervision.badge_db_etat >0 )
        {
            QString badge(30);
            badge = s;

            //if (lecteur.toInt()==ISO2 ) badge=badge_iso2(s);  // lecteur data_clock iso2
            //else badge=badge_wiegand(s);// lecteur wiegand

            QString j=QDate::currentDate().toString(Qt::ISODate);
            QString h=QTime::currentTime().toString(Qt::TextDate);

            if (!badge.compare("Badge inconnu"))
            {
                ihm_badge_inconnu(); //signal vers ihm erreur de bit badge
            }
            else
            {
                badge=extraire_matricule(badge);

                badge=badge.toUpper();
                badgeS=badge;
                badge_encode=encode_badge(badge);
                ihm_badge((QVariant) badgeS,(QVariant) j,(QVariant) h,(QVariant) badge_encode); //signal vers ihm
            }

        }

    }
    timer_timeout_badge->start(200);
}

QString Badge_qobject::rechercheNomPre(const QString &badge)
{
    badgelog->debug("rechercheNomPre");
    if(badge.isEmpty() || nomPre.isEmpty())
    {
        badgelog->info("rechercheNomPre badge vide");
        return QString();
    }else
    {
        badgelog->info("rechercheNomPre badge: %s", badge);
        if(nomPre.contains(badge))
            return nomPre.value(badge);
        else return QString("BADGE\nINCONNU");

    }
}

QString Badge_qobject::extraire_matricule(QString badge)
{
    badgelog->debug("extraire_matricule");
    int i,j;
    QString avant,apres;
    QString matricule;
    matricule=badge;
    if (!masque.isEmpty() && masque.contains('*'))
    {
        matricule.clear();
        i=masque.indexOf("*");
        j=masque.lastIndexOf("*");
        matricule=badge.mid(i,(j-i)+1);
    }

    if (!format.isEmpty() && format.contains('*'))
    {
        avant=format.section('*',0,0);
        apres=format.section('*',1,1);
        matricule.insert(0,avant);
        matricule.append(apres);
    }
    return(matricule);
}

QString Badge_qobject::badge_iso2(QString a)
{
    badgelog->debug("badge_iso2");
    /*****************************************************/
    /*  TRAIT_DATA_ISO2()                                */
    /*  Traitement des données magnétique ISO2 recues    */
    /*  Vérifie parités, START, STOP et LRC              */
    /*****************************************************/
    int i,d;
    int j=a.length();
    QBitArray bit(j+10);
    QBitArray lrc(5); // lrc = ou exclusif de la chaine STXxxxxETX
    bit.fill(false);
    lrc.fill(false);
    QString badge("Badge inconnu");

    for(i=0;i< j;i++)
    {
        if ((a.at(i))=='1') bit.setBit(i,true);
    }

    // recherche 0xB =Stx
    for(j=0;j< i;j++)
    {
        if (bit.testBit(j) && bit.testBit(j+1) && !(bit.testBit(j+2)) && bit.testBit(j+3)
                && !(bit.testBit(j+4))) {break;}
    }

    if (i==j)return(badge); // erreur pas de STX
    d=j;
    j+=5;

    QByteArray byte((j%5)+10,'\0');
    i=0;
    // empilage jusqu'a 0xF=Etx avec test parité impaire
    while(( !bit.testBit(j) || !bit.testBit(j+1) || !bit.testBit(j+2) || !bit.testBit(j+3) || !bit.testBit(j+4)) && j < a.length())
    {
        QBitArray parite(1);
        parite.setBit(0,bit.testBit(j+4));

        for(int k=0; k<4; k++)
        {
            parite.setBit(0,(parite.testBit(0) ^ bit.testBit(j+k)));
            byte[i] = ( byte.at(i) | ((bit.testBit(j+k))<<(k)));

        }
        if (!parite.testBit(0)) return(badge);
        byte[i] = byte.at(i) | 0x30;
        j+=5;
        i++;
    }
    if (j == a.length()) return(badge);
    byte[i]='\0';
    // ajouter ou exclusif avec ETX=0xF

    j+=10;
    for (i=d;i<j;i+=5)
    {
        for(int k=0; k<4; k++) lrc.setBit(k,(lrc.testBit(k)^ bit.testBit(i+k)));
    }

    // test  lrc checksum
    for(int k=0; k<4; k++) {if( lrc.testBit(k)) return(badge);}

    //Badge MIFARE ????
    if (!option.isEmpty())
    {
        unsigned long mat;
        switch (option.toInt())
        {
        case OPT_STANDARD: //  pas de transformation
            badge=QString(byte);
            break;
        case OPT_MIFARE_STID_HEXA : // conversion decimal -> 8 hexa majuscule
            mat=byte.at(0)*(unsigned int)16777216 + byte.at(1)*65536 + byte.at(2)*256 + byte.at(3);
            //          badge=QString("%1").arg(mat,8,16,QChar ('0'));
            badge=QString::number(mat,16).rightJustified(10,'0');
            break;
        case OPT_MIFARE_STID_INV :  // inversion octets forts/faibles + conversion decimal 10
            mat=byte.at(3)*(unsigned int)16777216 + byte.at(2)*65536 + byte.at(1)*256 + byte.at(0);
            badge=QString::number(mat,10).rightJustified(10,'0');
            break;
        default :
            break;
        }
    }
    else badge=QString(byte);

    return(badge);
}


QString Badge_qobject::badge_wiegand(QString a)
{
    badgelog->debug("badge_wiegand");
    int i;
    int j=a.length();
    QBitArray bit(j);
    bit.fill(false);

    badgelog->debug("Nb bits : %d", j);
    for(i=0;i< j;i++)
    {
        if ((a.at(i))=='1') bit.setBit(i,true);
    }

    BadgeLib badgelib;
    QString badge=badgelib.trait_motorola_hid(bit, option);
    return(badge);

}

/**
 * @brief (NOT USED)
 */
void Badge_qobject::transmitMsg_stid()
{
    badgelog->debug("transmitMsg_stid");
    message="ceci est un message";
    port->clear();
    port->write(message.toLatin1(),message.length());
}

/**
 * @brief (NOT USED)
 * @return
 */
int Badge_qobject::receiveMsg_stid()
{
    badgelog->debug("receiveMsg_stid");
    char buff[1024];
    int numBytes;

    while (port->waitForReadyRead(100));
    numBytes = port->bytesAvailable();

    if(numBytes > 1024) numBytes = 1024;

    int i = port->read(buff, numBytes);
    buff[i] = '\0';
    QString msg = buff;

    //        received_msg.append(msg);
    port->clear();
    return(numBytes);
}

// Lecteurs badge liaisons serie

void Badge_qobject::stid_legic_send(char * cmd,int timer_value)
{
    badgelog->debug("stid_legic_send");
    int i=0;
    char lrc=0;
    char cmd1[20];

    QString str;

    while (cmd[i] != char (0xFF))
    {
        cmd1[i] = cmd[i];
        lrc ^= cmd[i];
        QString x=QString("%1").arg(cmd1[i],2,16,QChar ('0'));
        str.append(x);
        i++;
    }
    cmd1[i]=lrc;
    badgelog->info(str.toStdString());

    port->clear();

    port->write(cmd1,i+1);
    while(!port->waitForBytesWritten(10));
    port->clear();
    timer_timeout_stid_legic->start(timer_value);

}

QString Badge_qobject::trait_data_stid_legic(char * data)
{
    badgelog->debug("trait_data_stid_legic");
    /*****************************************************/
    /*  TRAIT_DATA_SPECIF()                              */
    /*  Traitement des données lecteur specifique        */
    /*  Ex de trame lecture carte :                      */
    /* 0E 80 04 00 00 00 01 00 07 04 57 4B FB 5B 34      */

    /*  Entrees : _ lect =  pointeur sur struct lecteur  */
    /*  Sorties : _ aucune                               */
    /*****************************************************/

    QString badge("Badge inconnu");
    int i, len;
    unsigned char lrc;
    ulong mat=0;

    switch (option.toInt())
    {
    case LEGIC_CSN_DEC :
    case LEGIC_CSN_HEX :
    case LEGIC_CSN_DECINV :
    case LEGIC_CSN_HEXINV :
        if (data[9] != 4) return(badge);
        break;
    default :
        if (data[9] != 7) return(badge);
        break;
    }

    // test lrc
    len =int (data[0]);
    lrc = 0;
    for (i = 0; i < len; i++) lrc ^= data[i];
    if (lrc != data[len]) return(badge);

    badge.clear();
    switch(option.toInt())
    {
    case LEGIC_CSN_DEC :
        mat= data[13]*(unsigned int) 16777216 +data[12]*65536 + data[11]*256 +data[10];
        badge=QString::number(mat,10).rightJustified(10, '0');;
        break;
    case LEGIC_CSN_HEX :
        for (i=13;i>9;i--)
        {
            QString x=QString("%1").arg(data[i],2,16,QChar ('0'));
            badge.append(x);
        }
        break;
    case LEGIC_CSN_DECINV :
        mat= data[10]*(unsigned int)16777216 +data[11]*65536 + data[12]*256 +data[13];
        badge=QString::number(mat,10).rightJustified(10, '0');;
        break;
    case LEGIC_CSN_HEXINV :
        for (i=10;i<14;i++)
        {
            QString x=QString("%1").arg(data[i],2,16,QChar ('0'));
            badge.append(x);
        }
        break;
    default :
        for (i=10;i<17;i++)
        {
            QString x=QString("%1").arg(data[i],2,16,QChar ('0'));
            badge.append(x);
        }
        break;
    }
    return(badge);

}

void Badge_qobject::dialog_stid_legic(void)
{
    badgelog->debug("dialog_stid_legic");
    char cmd[20];
    char reponse[50];
    qint64 len,numBytes;
    QString str;
    badgelog->debug("dialog_phase=%d", dialog_phase);

    switch (dialog_phase)
    {
    case 0:       // attente evenement
        cmd[0]=0x03;
        cmd[1]=0x14;
        cmd[2]=0x01;
        cmd[4]=0xFF;
        dialog_phase=1;
        stid_legic_send(cmd,10);
        break;

    case 1:   // envoi Cde Bip STid
        cmd[0]=0x04;
        cmd[1]=0xCD;
        cmd[2]=0x0A;
        cmd[3]=0xBF;
        cmd[4]=0xFF;
        dialog_phase=2;
        stid_legic_send(cmd,10);
        break;

    case 2 :   // envoi def time out STID
        cmd[0]=0x03;
        cmd[1]=0xCE;
        cmd[2]=0x0A;
        cmd[4]=0xFF;
        dialog_phase=3;
        stid_legic_send(cmd,10);
        break;

    case 3 :   // envoi Cde Bip STid pour lecteur Advant
        cmd[0]=0x06;
        cmd[1]=0x74;
        cmd[2]=0x00;
        cmd[3]=0x00;
        cmd[4]=0x80;
        cmd[5]=0x85;
        cmd[6]=0xFF;
        dialog_phase=4;
        stid_legic_send(cmd,10);
        break;

    case 4:   // envoi Cde idle pour init lecteur
        presence_badge=99;
    case 5 :    //attente insertion carte envoi demande status
        cmd[0]=0x03;
        cmd[1]=0x30;
        cmd[2]=0x00;
        cmd[3]=0xFF;
        dialog_phase=6;
        dialog_timeout=0;
        stid_legic_send(cmd,10);
        break;

    case 6 :   // attend status
        numBytes = port->bytesAvailable();
        if (numBytes >0) // reception longueur
        {
            port->read(reponse,1);
            len= (char) reponse[0];
            numBytes = port->bytesAvailable();
            if (numBytes >= len) // reception reste message
            {
                port->read(reponse+1,len+1);
                str.clear();
                for (int j=0;j <len+1 ;j++){ QString x=QString("%1").arg(reponse[j],2,16,QChar ('0')); str.append(x);}
                badgelog->debug("REPONSE case 6 : %s", str);
                if (reponse[1]  == 0x30)
                {                                                   // retour = status = Erreur
                    if (presence_badge != (reponse[8]  & 0x10))
                    {
                        if (!(reponse[8]  & 0x10)) dialog_phase = 7;                 // NOMIM = 0 = carte preésente donc envoi commande lecture
                        else dialog_phase=5;                                        // carte pas dans le champ .. => vers attend insertion carte
                        presence_badge = (reponse[8]  & 0x10);
                    }
                    else dialog_phase=5;
                }
                else dialog_phase =4;                                   // reponse imprevue ... recommence dialogue lecteur
            }
            else dialog_phase= 4;                                     // reponse incomplete redemande status
            timer_timeout_stid_legic->start(1);
        }
        else
        {
            if (dialog_timeout++ > 10)
            {
                dialog_phase= 5;
                timer_timeout_stid_legic->start(1);
            }
            else timer_timeout_stid_legic->start(100);
        }

        break;

    case 7:   // envoi Cde idle pour init lecteur
        cmd[0]=0x03;
        cmd[1]=0x14;
        cmd[2]=0x01;
        cmd[3]=0xFF;
        dialog_phase=8;
        dialog_erreur=0;
        stid_legic_send(cmd,40);
        break;

    case 8 :    //attente reception message retour à init
        numBytes = port->bytesAvailable();
        if (numBytes >0) // reception longueur
        {
            port->read(reponse,1);
            len= (char) reponse[0];
            numBytes = port->bytesAvailable();
            if (numBytes >= len) // reception reste message
            {
                port->read(reponse+1,len+1);
                dialog_phase=9;
            }
            timer_timeout_stid_legic->start(1);
        }
        else timer_timeout_stid_legic->start(1);
        break;


    case 9 :   // envoi Cde lecture data
        cmd[0]=0x0A;    //NB octets de commande
        cmd[1]=0x80;    //Code commande lecture
        cmd[2]=0x00;
        if (option.toInt()==LEGIC_STX) cmd[3]=0x12;  // zone Code licence
        else cmd[3]=0x07; // zone N° serie
        if (option.toInt()==LEGIC_STX) cmd[4]=0x07;  // Longueur data à recevoir
        else cmd[4]=0x04;
        cmd[5]=0x01;   // periode de scan RF = 15x20ms = 0.3s
        cmd[6]=0x00;    // Pas de ctrl CRC
        cmd[7]=0x00;    // Pas de ctrl CRC
        if (option.toInt()==LEGIC_STX) cmd[8]=0x14; //MODE
        else cmd[8]=0x04;
        cmd[9]=0x01;
        cmd[10]=0xFF;
        dialog_phase=10;
        dialog_timeout=0;
        stid_legic_send(cmd,200);

        break;

    case 10 :  // Attend msg Time out STID ou lecture carte
        badgeS="Badge inconnu";
        numBytes = port->bytesAvailable();
        if (numBytes >0) // reception longueur
        {
            port->read(reponse,1);
            len= (char) reponse[0];
            numBytes = port->bytesAvailable();
            if (numBytes >= len) // reception reste message
            {
                port->read(reponse+1,len+1);
                str.clear();
                for (int j=0;j <len+1 ;j++){ QString x=QString("%1").arg(reponse[j],2,16,QChar ('0')); str.append(x);}
                badgelog->debug("REPONSE case 10 : %s", str);

                if (reponse[1] == 0x80)
                {
                    badgeS=trait_data_stid_legic(reponse);
                    dialog_phase=11;            //retour avec donnees
                }
                else if (reponse[0] == 0x6C && reponse[1] == 0x01 ) dialog_phase = 4;  // time out  recommence dialogue lecteur
                else if (reponse[1] == 0x30)   // retour = status = Erreur
                {
                    if (dialog_erreur < 2)
                    {              // on se donne droit a une erreur
                        dialog_erreur++;
                        dialog_phase =9;   // relecture immediate
                    }
                    else {dialog_erreur=0;dialog_phase=4;}
                }
                else dialog_phase =4; // reponse pas prevue !
            }
            else dialog_phase =4;
            timer_timeout_stid_legic->start(1);
        }
        else
        {
            if (dialog_timeout++ > 10)
            {
                dialog_phase= 9;
                timer_timeout_stid_legic->start(1);
            }
            else timer_timeout_stid_legic->start(100);
        }

        break;

    case 11 :    // attend prise en compte couche appli

        if (supervision_thread.supervision.http_etat >0 || supervision_thread.supervision.badge_db_etat >0 )
        {

            QString j=QDate::currentDate().toString(Qt::ISODate);
            QString h=QTime::currentTime().toString(Qt::TextDate);


            if (!badgeS.compare("Badge inconnu"))
            {
                ihm_badge_inconnu(); //signal vers ihm erreur de biture badge
            }
            else
            {
                badgeS=extraire_matricule(badgeS);
                badgeS=badgeS.toUpper();
                badge_encode=encode_badge(badgeS);
                ihm_badge((QVariant) badgeS,(QVariant) j,(QVariant) h,(QVariant) badge_encode); //signal vers ihm
            }

        }
        timer_timeout_stid_legic->start(1);
        dialog_phase=5; // vers attend retrait carte
        break;


    default :
        dialog_phase = 0;
        timer_timeout_stid_legic->start(1);
        break;
    }
}



QString Badge_qobject::trait_data_inside(char * data)
{
    /*****************************************************/
    /*  TRAIT_DATA_INSIDE()                              */
    /*  Traitement des données lecteur specifique        */
    /*  Carte a puce sans contact Mairie de Bordeaux     */
    /*                                                   */
    /*  Message recu du lecteur Inside, si carte Mifare  */
    /*   A4 03 51 A3 2F 24 00 00 00 00 90 00             */
    /*  avec :                                           */
    /*      A4 = Acquitte Cde SelectCard A4              */
    /*      03 = Protocole utilise par la carte = user   */
    /*           protocole                               */
    /*      51 A3 2F 24 = UID LSByte First               */
    /*      90 00 = SW1 SW2                              */
    /*  On controle :                                    */
    /*      - les 2 premiers octet = A4 03 -> Select Card*/
    /*        en user protocol = Mifare                  */
    /*      - la longueur de la reponse = 12             */
    /*      - si LSB UID != 88 (UID sur 4 octets)        */
    /*      - SW1 SW2 = 90 00                            */
    /*  On convertit le N° Serie Mifare (UID) en decimale*/
    /*  sur 10 caracteres apres remise dans l'orde       */
    /*  MSByte First :                                   */
    /*   24 2F A3 51 -> 0607101777                       */
    /*                                                   */
    /*  Message recu du lecteur Inside, si carte 14443B  */
    /*   A4 02 10 00 C6 8C 21 00 00 01 90 00             */
    /*  avec :                                           */
    /*      A4 = Acquitte Cde SelectCard A4              */
    /*      02 = Protocole ISO14443B niveau 3            */
    /*      10 00 C6 8C = PUPI MSByte First              */
    /*      90 00 = SW1 SW2                              */
    /*  On controle :                                    */
    /*      - les 2 premiers octet = A4 02 -> Select Card*/
    /*        en protocol ISO14443B                      */
    /*      - la longueur de la reponse = 12             */
    /*      - SW1 SW2 = 90 00                            */
    /*  On convertit le PUPI 14443B en decimale          */
    /*  sur 10 caracteres dans l'orde MSByte First :     */
    /*   10 00 C6 8C -> 268486284                        */
    /*                                                   */
    /*  Message recu du lecteur Inside, si carte 14443B' */
    /*    C2 C2 00 07 41 00 00 68 7F 90 00               */
    /*  avec :                                           */
    /*      C2 = Acquitte Cde Transmit C2                */
    /*      41 00 00 68 = Num Pupi MSByte First          */
    /*      90 00 = SW1 SW2                              */
    /*  On controle :                                    */
    /*      - le premier octet = C2 -> ISO14443 B prim   */
    /*      - la longueur de la reponse = 11             */
    /*      - SW1 SW2 = 90 00                            */
    /*  On convertit le PUPI 14443B' en decimale         */
    /*  sur 10 caracteres dans l'orde MSByte First :     */
    /*   41 00 00 68 -> 1090519144                       */
    /*                                                   */
    /*  Codes erreur retournees dans lect->result[0] :   */
    /*    1 = erreur longueur reponse                    */
    /*    2 = erreur SW1 SW2 != 90 00                    */
    /*    3 = 1er octet != A4 et != C2                   */
    /*    4 = 1er octet = A4 et 2nd octet inconnu        */
    /*    8 = carte 14443A avec UID > 4 octets, Desfire..*/
    /*    9 = carte pas decodee, cf Readerxopt           */
    /*                                                   */
    /*  Date de creation : 25/10/2005                    */
    /*  Modifications :                                  */
    /*      Dates       Objet                            */
    /* 11/08/09 Ajout decodage ISO14443B                 */
    /*                                                   */
    /*  Entrees : _ lect =  pointeur sur struct lecteur  */
    /*  Sorties : _ aucune                               */
    /*****************************************************/
    badgelog->debug("trait_data_inside");
    QString badge("Badge inconnu");
    quint64 longmat=0;
    ulong mat=0;


    switch (data[0])  // Decode en fonction du code commande utilise
    {
    case 0xA4 :  // Reponse a Cde Select Card 0x04
        if ( (data[10] != 0x90) || (data[11] != 0x00) ) return(badge);

        switch (data[1])   // Decode en fonction du protocole de la carte
        {
        case 0x02 :   // protocole ISO14443B
            if (option.toInt() & (int) OPT_NO_14443B) return(badge);   // pas de decodage carte ISO14443 B
            mat= data[2]* (unsigned int) 16777216 + data[3]*65536 + data[4]*256 +data[5];
            badge=QString::number(mat,10).rightJustified(10, '0');
            break;

        case 0x03 :   // user protocole = Mifare
            if (option.toInt() & (int) OPT_NO_MIFARE) return(badge);   // pas de decodage carte MIFARE (ISO14443A)
            mat= data[5]*(unsigned int) 16777216 + data[4]*65536 + data[3]*256 +data[2];  // inversion MSB<--> LSB
            badge=QString::number(mat,10).rightJustified(10, '0');
            if ( data[2] == 0x88 ) //Desfire 56bits
            {
                badge.clear();
                longmat= data[3] +(data[4]*0x100)+(data[5]*0x10000) + (data[6]* (unsigned int) 0x1000000)+ (data[7]*0x100000000)
                        + (data[8]*0x10000000000)+ (data[9]*0x1000000000000);
                badge=QString::number(longmat,10).rightJustified(16, '0');
            }
            break;

        default :
            return(badge);  // protocole inconnu
            break;
        }
        break;

    case 0xC2 :   // Carte ISO14443B prim
        if (option.toInt() & (int) OPT_NO_14443BP) return(badge);   // pas de decodage carte ISO14443 B
        if ( (data[8] != 0x90) || (data[9] != 0x00) ) return(badge);
        mat= data[3]*(unsigned int) 16777216 + data[4]*65536 + data[5]*256 +data[6];
        badge=QString::number(mat,10).rightJustified(10, '0');
        break;

    default :   // reponse inconnue
        return(badge);
        break;
    }
    return(badge);
}


void Badge_qobject::dialog_inside(void)
{
    badgelog->debug("dialog_inside");
    char cmd[20];
    char reponse[20];
    qint64 numBytes;
    QString str;
    int fin_att_ack=0;
    badgelog->debug("dialog_phase=%d", dialog_phase);

    switch (dialog_phase)
    {
    case 0:   // demande synchro liaison serie */
        if (!mifare_init)
        {
            cmd[0]=0x00;
            port->write(cmd,1);
            while(!port->waitForBytesWritten(10));
            dialog_phase=1;
        }
        else dialog_phase=2;
        timer_timeout_inside->start(50);
        break;

    case 1 :   // attente reponse synchro
        numBytes = port->bytesAvailable();
        if (numBytes >0)
        {
            mifare_init=1;   // synchro OK
            puce_type = INSID_SETSTAT_ISO14443A;
            presence_badge=0;
            port->clear();
            dialog_phase=0;
        }
        else {
            dialog_phase=0;
            mifare_init=0;
        }
        timer_timeout_inside->start(1);
        break;

    case 2:       // attente evenement
        if ((presence_badge & ISO14443A_ON1) | (presence_badge & ISO14443BP_ON)) dialog_phase=6; // carte encore dans le champ de l'antenne => attente retrait CARTE MIFARE
        else // carte Mifare ou B retiree du champ
        {
            cmd_index=0;
            if (puce_type == INSID_SETSTAT_ISO14443A) puce_type = INSID_SETSTAT_ISO14443BP;  // vers envoi setstatus 14443B Prim
            else  puce_type = INSID_SETSTAT_ISO14443A;  // vers envoi setstatus 14443B Prim
            dialog_phase=3;   // vers ENVOI setstatus
        }
        timer_timeout_inside->start(1);
        break;

    case 3 :   // envoi cde setstatus Mifare
        switch (puce_type)
        {
        case INSID_SETSTAT_ISO14443A:
            switch(cmd_index)
            {
            case 0:
                cmd[0]=0x80;
                cmd[1]=0xF4;
                cmd[2]=0x03;
                cmd[3]=0x5E;
                cmd[4]=0x01;
                break;
            case 1:
                cmd[0]=0x80;
                cmd[1]=0xF4;
                cmd[2]=0x03;
                cmd[3]=0x5F;
                cmd[4]=0x01;
                break;
            }
            break;
        case INSID_SETSTAT_ISO14443BP:
            switch(cmd_index)
            {
            case 0:
                cmd[0]=0x80;
                cmd[1]=0xF4;
                cmd[2]=0x03;
                cmd[3]=0x5E;
                cmd[4]=0x01;
                break;
            case 1:
                cmd[0]=0x80;
                cmd[1]=0xF4;
                cmd[2]=0x03;
                cmd[3]=0x5F;
                cmd[4]=0x01;
                break;
            case 2:
                cmd[0]=0x80;
                cmd[1]=0xF4;
                cmd[2]=0x03;
                cmd[3]=0x6F;
                cmd[4]=0x01;
                break;

            }
            break;
        }
        port->write(cmd,5);
        while(!port->waitForBytesWritten(10));
        dialog_phase=4;
        timer_timeout_inside->start(50);
        break;

    case 4:   // attente ack status + envoi setstatus data
        numBytes = port->bytesAvailable();
        if (numBytes >0) // reception longueur
        {
            port->read(reponse,numBytes);
            if (reponse[0] == INSID_SETSTATUS) // reception reste message
            {
                switch (puce_type)
                {
                case INSID_SETSTAT_ISO14443A:
                    switch(cmd_index)
                    {
                    case 0:
                        cmd[0]=0x32;
                        break;
                    case 1:
                        cmd[0]=0x12;
                        break;
                    }
                    break;
                case INSID_SETSTAT_ISO14443BP:
                    switch(cmd_index)
                    {
                    case 0:
                        cmd[0]=0x45;
                        break;
                    case 1:
                        cmd[0]=0x31;
                        break;
                    case 2:
                        cmd[0]=0x8A;
                        break;

                    }
                    break;
                }
            }
        }
        port->write(cmd,1);
        while(!port->waitForBytesWritten(10));
        dialog_phase=5;
        timer_timeout_inside->start(50);
        break;

    case 5 :    //attente ack data
        fin_att_ack=0;
        numBytes = port->bytesAvailable();

        port->read(reponse,numBytes);
        if (numBytes >=2) // reception des ack
        {
            if (reponse[0] == INSID_SW1_OK && reponse[1] == INSID_SW2_OK)  // ack
            {
                cmd_index++;
                switch (puce_type)
                {
                case INSID_SETSTAT_ISO14443A:
                    if (cmd_index ==2) fin_att_ack=1;

                    break;
                case INSID_SETSTAT_ISO14443BP:
                    if (cmd_index ==3) fin_att_ack=1;
                    break;
                }
            }
            if (!fin_att_ack) dialog_phase=3;  //vers envoi cde setstatus suivant
            else {cmd_index=0; dialog_phase=6;}
        }
        else
        {
            port->clear();
            dialog_phase=0;
        }
        timer_timeout_inside->start(1);
        break;

    case 6 :   // envoie cde selectcard
        switch (puce_type)
        {
        case INSID_SETSTAT_ISO14443A: // MIFARE
            cmd[0]=0x80;
            cmd[1]=0xA4;
            cmd[2]=0x00;
            cmd[3]=0x0C;
            cmd[4]=0x09;
            break;

        case INSID_SETSTAT_ISO14443BP: //ISO14443B
            cmd[0]=0x80;
            cmd[1]=0xC2;
            cmd[2]=0xF7;
            cmd[3]=0x07;
            cmd[4]=0x03;
            break;
        }
        port->write(cmd,5);
        while(!port->waitForBytesWritten(10));
        dialog_phase=7;
        timer_timeout_inside->start(100);
        break;

    case 7 :   // attente reponse selectcard en config Mifare ou ack en 1443B
        badgeS="Badge inconnu";
        numBytes = port->bytesAvailable();
        port->read(reponse,numBytes);
        str.clear();
        for (int j=0;j <numBytes ;j++){ QString x=QString("%1").arg(reponse[j],2,16,QChar ('0')); str.append(x);}
        badgelog->debug("Debut case 7 : %s", str);

        switch (puce_type)
        {
        case INSID_SETSTAT_ISO14443A: // MIFARE
            badgelog->debug("passage case 7 presence carte mifare ");
            if (numBytes ==12) // message reçu data MIFARE
            {
                if (!(presence_badge & ISO14443A_ON1))
                {
                    badgeS=trait_data_inside(reponse);
                    dialog_phase=99; // Enregistre badge
                }
                else dialog_phase=0; // carte deja detecte => meme traitt que si pas de cart
                presence_badge= (ISO14443A_ON1 | ISO14443A_ON2);   // enregistre presence carte pour 2 polling sans
            }
            else
            {
                if (numBytes ==2) // message reçu
                {
                    if ((reponse[0] == INSID_SW1_NOCARD || reponse[0] == INSID_SW1_CARDNI) && reponse[1] == INSID_SW2_NOCARD) //pas de carte OU carte Non Identifiee
                    {
                        if (presence_badge & ISO14443A_ON2) presence_badge &= ~ISO14443A_ON2; // 1er polling sans carte ISO14443A dans le champ
                        else presence_badge=0; // pas de carte presente
                    }
                }
                else
                {
                    mifare_init = 0; // demande synchro liaison serie
                    presence_badge=0;
                }
                dialog_phase=0;
            }
            break;


        case INSID_SETSTAT_ISO14443BP:  //ISO14443B'
            badgelog->debug("passage case 7 presence carte B'' ");
            if (numBytes >=1) // message reçu
            {
                if (reponse[0] == 0xC2) dialog_phase=8; //ack select card 1444
                else
                {
                    mifare_init=0;
                    dialog_phase=0;
                }
            }
            else
            {
                mifare_init=0;
                dialog_phase=0;
            }
            break;
        }
        timer_timeout_inside->start(1);
        break;

    case 8:  //envoi demande data en config 14443B Prim
        cmd[0]=0x00;
        cmd[1]=0x0B;

        cmd[2]=0x7F;
        port->write(cmd,3);
        while(!port->waitForBytesWritten(10));
        dialog_phase=9;
        timer_timeout_inside->start(100);
        break;

    case 9 :    //attente reception message t
        numBytes = port->bytesAvailable();
        port->read(reponse,numBytes);
        str.clear();
        for (int j=0;j <numBytes ;j++){ QString x=QString("%1").arg(reponse[j],2,16,QChar ('0')); str.append(x);}
        badgelog->debug("REPONSE case 9 : %s", str);

        if (numBytes ==10) // message reçu
        {
            if (!(presence_badge & ISO14443BP_ON))
            {
                presence_badge= ISO14443BP_ON;   // enregistre presence carte
                badgeS=trait_data_inside(reponse);
                dialog_phase=99; // carte jamais détectée
            }
            else dialog_phase=0; // carte deja detecte => meme traitement que si pas de carte
        }
        else
        {
            if (numBytes ==2) // message reçu
            {
                if ((reponse[0] == INSID_SW1_NOCARD || reponse[0] == INSID_SW1_CARDNI) && reponse[1] == INSID_SW2_NOCARD) //pas de carte OU carte Non Identifiee
                    presence_badge = 0; // pas de carte ISO14443B prim dans le champ
            }
            else
            {
                mifare_init = 0; // demande synchro liaison serie
                presence_badge=0; // reponse incorrecte -> chge type carte
            }
            dialog_phase=0;
        }
        timer_timeout_inside->start(1);
        break;

    case 99 :    // attend prise en compte couche appli
        if (supervision_thread.supervision.http_etat >0 || supervision_thread.supervision.badge_db_etat >0 )
        {
            QString j=QDate::currentDate().toString(Qt::ISODate);
            QString h=QTime::currentTime().toString(Qt::TextDate);

            if (!badgeS.compare("Badge inconnu"))
            {
                ihm_badge_inconnu(); //signal vers ihm erreur de biture badge
            }
            else
            {
                badgeS=extraire_matricule(badgeS);
                badgeS=badgeS.toUpper();
                badge_encode=encode_badge(badgeS);
                ihm_badge((QVariant) badgeS,(QVariant) j,(QVariant) h,(QVariant) badge_encode); //signal vers ihm
            }
        }
        dialog_phase=0; // vers attend retrait carte
        timer_timeout_inside->start(1);

        break;

    default:   // envoi Cde lecture data
        dialog_phase=0;
        mifare_init=0;
        timer_timeout_inside->start(1);
        break;
    }

}

/*-------------- Codage badge pour accès intranet  -----------*/
QString Badge_qobject::encode_badge(QString badge)
{
    badgelog->debug("encode_badge");
    QString a=QString::number(QDateTime::currentMSecsSinceEpoch());
    a.append(" "+badge);
    QByteArray b;
    badgelog->debug("Badge: %s", a.toStdString());

    if (supervision_thread.supervision.key_crypt.isEmpty())
    {
        b=a.toLatin1().toBase64();
    }
    else
    {
        using namespace CryptoPP;
        using namespace std;
        Encryption *encry = new Encryption();
        string msg= encry->encryption(supervision_thread.supervision.key_crypt, a, "AES", true);
        b=QByteArray(msg.c_str(),msg.length());
        //     QByteArray c=QString::fromStdString(msg).toLatin1();
        //     badgelog->debug("badge encode: " + c);
        //      c=c.toPercentEncoding();
        /*     msg = encry->encryption(key_crypt, b, "AES", false);
        QString x= QString::fromStdString(msg);
        badgelog->debug("badge decode: " + x);
*/
    }
    return(QString(b));
}

void Badge_qobject::customEvent ( QEvent * event )
{
    badgelog->debug("customEvent");
    if(T2bEvent::custom_type()== event->type())
    {
        T2bEvent *e = (T2bEvent *)event;

        switch (e->eventType())
        {
        case EventType::BADGE_UPDATE_NOM:
            updateNomPre();
            break;
        default:
            break;
        }
    }
}

void Badge_qobject::morpho_getbaseconfig(void)
{
    badgelog->debug("morpho_getbaseconfig");
    QMetaObject::invokeMethod(&morpho, "MORPHO_GetBaseConfig", Qt::QueuedConnection);
}


void Badge_qobject::morpho_message(QString message)
{
    badgelog->debug("morpho_message");
    ihm_morpho_message((QVariant) message);
}

void Badge_qobject::morpho_enrolluser(QString identifiant)
{
    badgelog->debug("morpho_enrolluser");
    FILE *fp;
    size_t lSize;
    QString id;
    int l=identifiant.length();
    fp = fopen ( "/var/x7/contents/biometrie/minutiae.bio" , "r" );
    if (fp !=NULL)
    {
        // obtain file size:
        fseek (fp , 0 , SEEK_END);
        lSize = ftell(fp);
        rewind (fp);
        // copy the file into the buffer:
        if (lSize >3) fread(&l,sizeof(char),1,fp);
        fclose (fp);
    }

    id=identifiant.rightJustified(l,'0',true);
    morpho.identifiant[0]='\0';
    memcpy( morpho.identifiant,id.toLatin1().constData(),10);
    QMetaObject::invokeMethod(&morpho, "MORPHO_EnrollUser", Qt::QueuedConnection);
}

void Badge_qobject::morpho_deleteuserindex(QString index)
{
    badgelog->debug("morpho_deleteuserindex");
    bool ok;
    morpho.index=index.toULong(&ok,10);
    if (ok) QMetaObject::invokeMethod(&morpho, "MORPHO_DeleteUserIndex", Qt::QueuedConnection);
    QFile file ("/var/x7/contents/biometrie/minutiae.bio");
    if (file.exists())
    {
        file.copy("/var/x7/contents/biometrie/minutiae_save.bio");
        file.remove();
    }
}

void Badge_qobject::morpho_deletealluser(void)
{
    badgelog->debug("morpho_deletealluser");
    QMetaObject::invokeMethod(&morpho, "MORPHO_DeleteAllUser", Qt::QueuedConnection);
    QFile file ("/var/x7/contents/biometrie/minutiae.bio");
    if (file.exists())
    {
        file.copy("/var/x7/contents/biometrie/minutiae_save.bio");
        file.remove();
    }
}

void Badge_qobject::morpho_cancel(void)
{
    badgelog->debug("morpho_cancel");
    MSO_Cancel(morpho.i_h_Mso100Handle);
}

void Badge_qobject::morpho_destroy_database(void)
{
    badgelog->debug("morpho_destroy_database");
    QMetaObject::invokeMethod(&morpho, "MORPHO_DestroyDataBase", Qt::QueuedConnection);
    QFile file ("/var/x7/contents/biometrie/minutiae.bio");
    if (file.exists())
    {
        QFile::remove("/var/x7/contents/biometrie/minutiae_save.bio");
        file.copy("/var/x7/contents/biometrie/minutiae_save.bio");
        file.remove();
    }
}

void Badge_qobject::morpho_reload_database(void)
{
    badgelog->debug("morpho_reload_database");
    QMetaObject::invokeMethod(&morpho, "MORPHO_FillDataBase", Qt::QueuedConnection);
    QFile file ("/var/x7/contents/biometrie/minutiae_save.bio");
    if (file.exists())
    {
        QFile::remove("/var/x7/contents/biometrie/minutiae.bio");
        file.copy("/var/x7/contents/biometrie/minutiae.bio");
    }
}

void Badge_qobject::morpho_create_database(void)
{
    badgelog->debug("morpho_create_database");
    QMetaObject::invokeMethod(&morpho, "MORPHO_CreateDataBase", Qt::QueuedConnection);
}

void Badge_qobject::morpho_stop(void)
{
    badgelog->debug("morpho_stop");
    timer_timeout_morpho->stop();
    morpho_run=0;
}

void Badge_qobject::morpho_start(void)
{
    badgelog->debug("morpho_start");
    //    ihm_morpho_status((QVariant) 1); //signal vers ihm database empreintes non vide
    morpho_run=1;
    timer_timeout_morpho->start(1);
}

/* TRAITEMENT Lecteur EMPREINTE MORPHOSMART MS0-CBM   */

void Badge_qobject::dialog_morpho(void)
{
    badgelog->debug("dialog_morpho");
    QFile file ("/var/x7/contents/biometrie/time2bii.bio");
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QFile::remove("/var/x7/contents/biometrie/minutiae_save.bio");
            QFile::remove("/var/x7/contents/biometrie/minutiae.bio");
            file.copy("/var/x7/contents/biometrie/minutiae_save.bio");
            file.copy("/var/x7/contents/biometrie/minutiae.bio");
            file.close();
            file.remove();
            QMetaObject::invokeMethod(&morpho, "MORPHO_FillDataBase", Qt::QueuedConnection);
            morpho_run=1;
            timer_timeout_morpho->start(2000);
        }
        else
        {
            file.remove();
            if (morpho_run) timer_timeout_morpho->start(1);
        }
    }
    else{
        QMetaObject::invokeMethod(&morpho, "MORPHO_Identify", Qt::QueuedConnection);
        //      QMetaObject::invokeMethod(&morpho, "MORPHO_GetBaseConfig", Qt::QueuedConnection);
    }
}


void Badge_qobject::morphoOUwiegand_badge(QString badge)
{
    badgelog->debug("morphoOUwiegand_badge");
    timer_timeout_morpho->stop();
    if (supervision_thread.supervision.http_etat >0 || supervision_thread.supervision.badge_db_etat >0 )
    {
        QString j=QDate::currentDate().toString(Qt::ISODate);
        QString h=QTime::currentTime().toString(Qt::TextDate);

        if (!badge.compare("Badge inconnu"))
        {
            ihm_badge_inconnu(); //signal vers ihm erreur de bit badge
            if (morpho_run) timer_timeout_morpho->start(2000);
        }
        else
        {
            if (!badge.compare("Base Vide"))
            {
                ihm_morpho_status((QVariant) 0); //signal vers ihm database empreintes vide
                if (morpho_run) timer_timeout_morpho->start(5000);
            }
            else
            {
                if (!badge.compare("ERREUR"))
                {
                    badgelog->debug("ERREUR");
                    ihm_morpho_status((QVariant) 0); //signal vers ihm probleme lecteur morpho
                    MSO_Cancel(morpho.i_h_Mso100Handle);
                    if (morpho_run) timer_timeout_morpho->start(1000);
                }
                else
                {
                    if (badge.compare("No Badge") )
                    {
                        badgeS=extraire_matricule(badge);
                        badgeS=badgeS.toUpper();
                        badge_encode=encode_badge(badgeS);
                        ihm_badge((QVariant) badgeS,(QVariant) j,(QVariant) h,(QVariant) badge_encode); //signal vers ihm
                    }
                    badgelog->debug("Nobadge");
                    ihm_morpho_status((QVariant)1);
                    if (morpho_run) timer_timeout_morpho->start(1);
                }
            }
        }
    }
    else {if (morpho_run) timer_timeout_morpho->start(1000);}
}


void Badge_qobject::morphoETwiegand_badge(QString badge)
{
    badgelog->debug("morphoETwiegand_badge");
    timer_timeout_morpho->stop();
    if (supervision_thread.supervision.http_etat >0 || supervision_thread.supervision.badge_db_etat >0 )
    {
        QString j=QDate::currentDate().toString(Qt::ISODate);
        QString h=QTime::currentTime().toString(Qt::TextDate);

        if (!badge.compare("Badge inconnu"))
        {
            ihm_badge_inconnu(); //signal vers ihm erreur de bit badge
            badgeS.clear();
            badgeS_morpho.clear();
            if (morpho_run) timer_timeout_morpho->start(2000);
        }
        else
        {
            if (!badge.compare("Base Vide"))
            {
                ihm_morpho_status((QVariant) 0); //signal vers ihm database empreintes vide
                if (morpho_run) timer_timeout_morpho->start(5000);
            }
            else
            {
                if (!badge.compare("ERREUR"))
                {
                    badgelog->debug("ERREUR");
                    ihm_morpho_status((QVariant) 0); //signal vers ihm probleme lecteur morpho
                    MSO_Cancel(morpho.i_h_Mso100Handle);
                    if (morpho_run) timer_timeout_morpho->start(1000);
                }
                else
                {
                    if (badge.compare("No Badge") )
                    {
                        badgeS=extraire_matricule(badge);
                        badgeS=badgeS.toUpper();
                        badge_encode=encode_badge(badgeS);
                        if(badgeS_morpho.isEmpty())
                        {
                            ihm_badge_message((QVariant) QObject::trUtf8("Présenter badge"));
                            timer_attente_badge_morpho->start(200);
                        }
                    }
                    else //TIME OUT
                    {
                        if(!badgeS_morpho.isEmpty() && !badgeS.isEmpty())
                        {
                            if (morpho_egal_badge)
                            {
                                if(!badgeS_morpho.compare(badgeS)) ihm_badge((QVariant) badgeS_morpho,(QVariant) j,(QVariant) h,(QVariant) badge_encode_morpho);
                                else ihm_badge_inconnu(); //signal vers ihm erreur de bit badge
                            }
                            else ihm_badge((QVariant) badgeS_morpho,(QVariant) j,(QVariant) h,(QVariant) badge_encode_morpho);
                            badgeS_morpho.clear();
                            badgeS.clear();
                        }
                        else timer_attente_badge_morpho->start(200);
                    }
                }
                badgelog->debug("Nobadge");
                ihm_morpho_status((QVariant)1);
                if (morpho_run)timer_timeout_morpho->start(1);
            }
        }
    }
    else {if (morpho_run) timer_timeout_morpho->start(1000);}
}


// QTHREAD
void Badge_qthread::init(void)
{
    badgeThreadlog->debug("init");
}

void Badge_qthread::run()
{
    badgeThreadlog = &log4cpp::Category::getInstance("Badge_qthread");
    badgeThreadlog->debug("run");
    badge.init();
    exec();
}
