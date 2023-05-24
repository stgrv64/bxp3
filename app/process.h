/**
 * @file
 * @brief Process_qobject and Process_qthread class declaration
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


#ifndef PROCESS_H
#define PROCESS_H

#include <QMetaObject>
#include <QApplication>
#include <QVariant>
#include <QObject>
#include <QThread>
#include <QString>
#include <QEvent>
#include <QTimer>
#include <QProcess>
#include <QtSql>
#include "qserialport.h"
#include "qserialportinfo.h"

#include <QKeyEvent>

#include <QObject>
#include <QScopedPointer>

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>


//FIXME Pourquoi cette class s'appelle Process_qobject ? (BG: merci steriflow)
//Que gere cette class :
//cle usb
//serveur temps NTP
//ecran
//Batterie
//Reboot ?
//ports serie
//interface reseau
//audio
//test automatique
//init lecteur badge
//configuration touches
/**
 * @brief The Process_qobject class
 */
class Process_qobject : public QObject
{
 Q_OBJECT

public:
   Process_qobject(void);
   void process_init(void);
   static log4cpp::Category * log4cpp;
   QString serveur_ntp;
   QString cadence_ntp;
   QString timezone;
   QString langue;

   QString type_ecran;
   QString backlight_N1;
   QString backlight_N2;
   QString sleep_mode;
   QString delai_N2;
   QString delai_OFF;
/*   int delai_N2;
   int delai_OFF;
  */
   int audio_volume;

   void process_init_lecteur_badge(void); // utilisé dans process_set_lecteur_badge()
   void process_init_ntp(void); // utilisé dans process_set_ntp
   void process_init_divers(void); //utilisé dans process_set_divers
   void process_init_wifi(void); //NOT USED
   void process_init_3G(void);
   void process_init_SSH(void);
   void process_init_SwUpdate(void);

public slots:
    void process_init0(void);
    void wake_up(void);
    void time2bii_tests(int a); //appel test_usb, test ethernet, test audio, test_backlight
    void test_uart(QString port,int a); //NOT USED
    void test_usb(QString port,int a);
    void test_ethernet(int a);
    void test_io(QString test,int a); //NOT USED
    void test_audio(int a);
    void test_backlight(int a);
    void test_sleep_mode(int a);

    void process_set_nom(QString nom); // NOT USED
    void process_set_network(QString interface,QString ip,QString netmask,QString gateway); //NOT USED
    void process_set_mode_network(int mode,QString nom,QString ip,QString netmask,QString gateway,QString dns1, QString dns2); //NOT USED
    void process_set_lecteur_badge(QString lecteur,QString option,QString masque,QString format);
    void process_set_http(QString serveur,QString https,QString http,QString service,QString user,QString password,QString timeout);
    void process_set_ntp(QString serveur,QString cadence,QString timezone,QString langue);
    void process_set_divers(QString ecran,QString bl1,QString delai,QString bl2,QString delai_off,QString sleep_mode);
    void process_set_wifi(QString ssid, QString psk);
    void process_set_3G(QString apn,QString pin ,QString phone,QString user,QString password);
    void process_set_SSH(QString start_stop);
    void process_set_SwUpdate(QString start_stop);
    void set_touches_fonction(QString libelle,QString activite,QString url,QString proxy,QString tempo);
    void qml_message(QString s);
    void process_browser(QString url,QString proxy,QString audio,QString tempo);
    void display_message(QString titre1,QString texte1,QString titre2,QString texte2,QString texte2_1,QString titre3,QString texte3,QString texte3_1,QString c1,QString c2,QString c3,QString c4,QString badge,QString audio,QString tempo);
    void video(QString nom,QString tempo);

    void process_init_mode_network(); //NOT USED
    void process_init_http(void); //utilisé dans process_set_http et appelé par supervision

signals:
    //void tension_batterie(QVariant);
    void signal_ihm_usbkey_status(QVariant);
    void signal_ihm_retour_test(QVariant,QVariant);
    void signal_badge_sleep_mode();  //NOT USED


    //void signal_init_mode_network(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant);//obsolete
    //void signal_init_wifi(QVariant,QVariant);//obsolete
    void signal_ihm_init_lecteur_badge(QVariant,QVariant,QVariant,QVariant);
    void signal_ihm_init_http(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant);
    void signal_ihm_init_ntp(QVariant,QVariant,QVariant,QVariant);
    void signal_ihm_init_divers(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant);
    void signal_ihm_init_3G(QVariant,QVariant,QVariant,QVariant,QVariant);
    void signal_ihm_init_SSH(QVariant);
    void signal_ihm_init_SwUpdate(QVariant);
    void signal_window_init_orientation_ecran(QVariant);
    void signal_window_set_touchesfonction(void);
    //void signal_thread_exit_start(int);//obsolete

    void signal_ihm_browser(QVariant,QVariant,QVariant,QVariant);
    void signal_video(QVariant,QVariant);
    void signal_display_message(QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant,QVariant);

    // nouveau signal time2bii-1.12
    void signal_supervision_init_http(void);
    void signal_supervision_http_status(int);//test
    void signal_X7GestLecteur_initLecteur(void);
    void signal_window_restart_app(void);

private:
   void delayBackLight();

   QSqlDatabase process_badge_db;
   int usbkey_mounted;
   QTimer *timer_init;
   QTimer *timer_orientation;
   QTimer *timer_ntp;
   QTimer *timer_reboot;
   QTimer *timer_rebootauto;
   QTimer *timer_restartauto;
   QTimer *timer_usbkey;
   QTimer *timer_initprocess;
   QTimer *timer_tar;
//   int batterie_mesure;

   QProcess qProcntp;
   int batterie;
   int sleep_mode_etat;

   QTimer *timer_batterie;
   QTimer *timer_sleep_mode_control;

   // serial
   QString received_msg;
   QString message;
   QSerialPort *port;
   QSerialPort *port_test;

private slots:
    void priv_sleep_mode_control(void);
    void priv_refresh_batterie(void);

    QString priv_process_test_interface(); //not used
    bool priv_process_test_ip();  //not used, call priv_process_test_interface
    void priv_test_lecteur(int a);
    void priv_test_relay(int a);

    void priv_set_date_heure(QString an,QString mois,QString jour,QString heure,QString minute,QString seconde);
    void priv_set_timezone_langue(int mode,QString timezone,QString langue);

    void priv_ts_calibrate_init(void);
    void priv_ts_calibrate(void);

    // serial
    void priv_transmitMsg(QSerialPort *p);
    void priv_receiveMsg(QSerialPort *p);

    void priv_init_process(void); //not used
    void priv_refresh_ntp(void); //not used, call priv_process_test_ip
    void priv_refresh_reboot(void); //not used
    void priv_refresh_rebootauto(void); //not used
    void priv_refresh_restartauto(void); //not used
    void priv_refresh_usbkey(void);    //not used
    void priv_audio_set_volume(QString a); //not used

};

/**
 * @brief The Process_qthread class
 */
class Process_qthread : public QThread
{

public:
    void proc_qthread_init(void); //init badge_thread et supervision_thread (NOT USED)
    void run();
    Process_qobject process;

};
 
#endif // PROCESS_H

GLOBAL Process_qthread process_thread;
