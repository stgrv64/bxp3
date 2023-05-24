/**
 * @file
 * @brief X7GestConfig class declaration
 ******************************************/
#ifndef X7GESTCONFIG_H
#define X7GESTCONFIG_H

#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <QVariant>
#include <QString>
#include <QStringList>
#include <QtCore/QCoreApplication>
//#include <QtSerialPort/QSerialPort>
#include "qserialport.h"
#include <log4cpp/Category.hh>

#define QPOST_EVENT_GESTCONFIG(event) QCoreApplication::postEvent((QObject*)X7GestConfig::getInstance(),event);

class X7PortSerie;

class X7GestConfig : public QObject
{
    Q_OBJECT
public:
    explicit X7GestConfig(QObject *parent = nullptr);
    static X7GestConfig * getInstance();

    bool ouvrirBdd(const QString &bddName);
    bool fermeBdd(QString bddName); //not used
    void customEvent( QEvent * event ); //not used

    static QString bddSetupName;
    static QString bddBadgeName;
    static X7GestConfig * instance;
    static log4cpp::Category * log4cpp;

    QSerialPort *port;
    quint32 getIpLocal();
    bool dhcp();

    void x7GestConfig_set_lecteur_badge(QString lecteur, QString option, QString masque, QString format); //NOT USED
    void x7GestConfig_init_lecteur_badge(); //NOT USED
    X7PortSerie *getPortSerieLecteur() const;
    X7PortSerie *getPortSerieServiceDevice() const;
    bool readConfig();
    QString getConnmanWiredService();
    void x7GestConfig_init_divers(void);

    enum modeIpV4 {DHCP,FIX};
    struct configIpV4
    {
        QString ip;
        QString netmask;
        QString gateway;
        QStringList dns;
    };

signals:
    // signal_ihm_init_wifi à corriger et tester
    void signal_ihm_init_wifi(QVariant,QVariant);
    //repris pour l'IHM
    void signal_ihm_init_mode_network(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant);

public slots:
    void startX7GestConfig();
    void updateDateTimeNtp();
    void setHostName(QString name);
    void setIpV4Config(modeIpV4 mode, configIpV4 config);
    void x7GestConfig_setWifi(QString ssid,QString psk);
    void x7GestConfig_initWifi();
    void x7GestConfig_set_mode_network(int mode, QString nom, QString ip, QString netmask, QString gateway, QString dns1, QString dns2);
    void x7GestConfig_init_mode_network();
    void setAudioVolume(QString a);

private:

    void initTsCalibrate(); //not used
    void initHostName();
    void tsCalibrate(); //not used
    void privUpdateDateTime(const QDateTime *dateTime); //not used
    void quitter();

    int _audioVolume;
    QTimer *timer_ntp;

    X7PortSerie *_portSerieLecteur;
    X7PortSerie *_portSerieServiceDevice;

    QString serveur_ntp;
    QString cadence_ntp;

    QString binaryOctetsToDecimal(QString &IPbinary);
};

#endif // X7GESTCONFIG_H
