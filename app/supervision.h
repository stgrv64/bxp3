/**
 * @file
 * @brief Supervision_qobject and Supervision_qthread class definition
*******************************************************************/
/***************************************************************************
 *   Copyright (C) 2007 by michel haget                                    *
 *   mhaget@taillon                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifdef DEFINE_GLOBAL
#define GLOBAL
#define INIT(x)=x
#else
#define GLOBAL extern
#define INIT(x)
#endif

#ifndef SUPERVISION_H
#define SUPERVISION_H

#include "Hashes.h"
#include "Encryptions.h"
#include "Conversions.h"
#include "BruteForce.h"

#include <QMetaObject>
#include <QApplication>
#include <QVariant>
#include <QObject>
#include <QThread>
#include <QString>
#include <QEvent>
#include <QList>
#include <QTimer>
#include <QTest>

#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkInterface>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QImage>
#include <QImageReader>
#include <QAuthenticator>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QSslCertificate>
#include <QEventLoop>
#include <QSslKey>
#include <QSslError>
#include <QFile>
#include <QtSql>
#include <QLocale>
#include <QDateTime>
//#include <qtconcurrentrun.h>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

/**
 * @brief The badgeage class
 */
struct badgeage
{
    QString csn;
    QDateTime datetime;
    QString activite;
    int dbKey;

    badgeage():csn(QString()),datetime(QDateTime()),activite(QString()),dbKey(0){}
    badgeage(QString csn, QDateTime datetime, QString activite, int dbKey):csn(csn),datetime(datetime),activite(activite),dbKey(dbKey){}

    QString toString()
    {
        QString str = this->csn+" "+dateToString()+" "+timeToString()+" "+this->activite;
        return str;
    }

    QString dateToString()
    {
        return this->datetime.date().toString(Qt::ISODate);
    }

    QString timeToString()
    {
        return this->datetime.time().toString(Qt::TextDate);
    }
};

#define QPOST_EVENT_SUPERVISION(event) QCoreApplication::postEvent((QObject*)Supervision_qobject::getInstance(),event);

extern log4cpp::Category *supervisionlog;

//FIXME pourquoi cette class s'appelle Supervision_qobject ?
//Que gere cette class :
//serveur http WS config ?
//Interface reseau ? encore ?
//badges <-- principale
//nom de la t2b ?
/**
 * @brief The Supervision_qobject class
 */
class Supervision_qobject : public QObject
{
    Q_OBJECT
public:
    Supervision_qobject(void);
    static Supervision_qobject * getInstance();
    static Supervision_qobject * instance;

    static const QString version_logiciel_i;
    QString version_logiciel;
    QString key_crypt;

    void supervision_init(void);
    bool post_get(QUrl*,QString );
//    bool post(QUrl*);

    void supervision_badge_db_status(int);

    QByteArray encode_string(QByteArray  s);

    QString interface_name;
    QHostAddress ip;
    int badge_db_etat;
    int http_etat;
    QString http_server;
    QString http_port_https,http_port_http,http_service,http_user,http_password, http_timeout;
    int http_erreurs;
    QString time2bii_name;

    void sauvegardeBadge(int id);
    void demarreTrigger(int high, int low);

    QString getTime2bii_name() const;
    void setTime2bii_name(const QString &value);

    void customEvent(QEvent *event);


signals:
    void signal_ihm_http_status(QVariant);
    void signal_ihm_badge_db_status(QVariant);
    void signal_display_message(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant);
    void signal_x7GestConfig_init_mode_network(void);
    void signal_process_init_http(void);

public slots:
     void ihm_post_badge(QString,QString,QString,QString);
     void supervision_init_http(void);
     void supervision_http_status(int);

private slots:
    void priv_trigger(void);
    void provideAuthentication(QNetworkReply *reply, QAuthenticator *ator);
    void finishedSlot(QNetworkReply* reply);
    void supervision_init0(void);
    void priv_restart_reseau(void);
    QString priv_supervision_test_interface();
    void priv_supervision_test_ip();
    void priv_attente_ip(int ok);
    void attente_ssl_certificat(void);

private:
    quint32 nextIdPost();
    QSqlDatabase badge_db;
    QNetworkAccessManager* manager;
    QNetworkReply* networkreply;
    QNetworkRequest* request;
    QSslConfiguration* SslConfiguration;
    QTimer timerloop;
    quint32 idPost;

    QCache<quint32,badgeage> badgeCache;


    QTimer *Stimer;
    QUrl url;
    QFile ssl_certificat;
    QString reply_string;
    int time_out;
    int http_initok;

};


//FIXME ne pas utiliser des objets intermediaires Supervision_qthread -> Supervision_qobject
/**
 * @brief The Supervision_qthread class
 */
class Supervision_qthread : public QThread
{

public:

   void supervision_qthread_init(void);
   void run();

   Supervision_qobject supervision;

private:
};

Q_DECLARE_METATYPE(badgeage)

#endif // SUPERVISION_H

GLOBAL Supervision_qthread supervision_thread;

