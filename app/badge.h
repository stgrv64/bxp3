/***********************************************************************
*   Copyright (C) 2007 by michel haget                                     *
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
/**
 * @file
 * @brief Badge_qobject and Badge_qthread class declaration
*******************************************************************/


#ifdef DEFINE_GLOBAL
#define GLOBAL
#define INIT(x)=x
#else
#define GLOBAL extern
#define INIT(x)
#endif

#ifndef BADGE_H
#define BADGE_H


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
#include <QTimer>
#include <QDate>
#include <QDateTime>
#include <QFile>
#include <QIODevice>
#include <QProcess>
#include <QBitArray>
#include <QtSql>
//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>
#include "qserialport.h"
#include "qserialportinfo.h"

#include "morpho.h"
#include"MSO100.h"

#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

//extern UC	g_uc_ExportScore;
//extern int ConvertError(UC i_uc_Status);

/* Type de lecteur */
#define  NO_READER          0
#define  ISO2               1      // ISO2 data/clock
#define  WIEGAND            2      // WIEGAND
#define  STID_LEGIC         3      // Lecteur serie rs232 STID legic
#define  INSIDE             4      // Lecteur serie TTL INSIDE CONTACTLESS MG220
#define  MORPHOouWIEGAND     5      // Capteur d'empreintes Sagem Morpho CBM ou lecteur WIEGAND
#define  MORPHOetWIEGAND     6      // Capteur d'empreintes Sagem Morpho CBM et lecteur WIEGAND

/* Valeur de lect->option sur lecteur ISO2  */
/* Pour simplifier la tache des installateurs, on reprend la numerotation Integrale */
#define OPT_STANDARD       0  /* lecture et decodage format ISO2 */
#define OPT_MIFARE_STID_HEXA  1  /* numero serie mifare recu en 13 decimal, convertit en 8 hexa */
#define OPT_ISO1              2  /* lecture et decodage format ISO1 */
#define OPT_MIFARE_STID_INV   3  /* numero serie mifare recu en 13 decimal, inversion octet fort/faible + conversion 10 decimal */



// valeur de option pour lecteur STID LEGIC
#define LEGIC_CSN_DEC     0   // decodage decimal
#define LEGIC_CSN_DECINV  1   // decodage decimal inverse
#define LEGIC_CSN_HEX     2   // decodage hexadecimal
#define LEGIC_CSN_HEXINV  3   // decodage hexadecimal inverse
#define LEGIC_STX         4   // decodage hexadecimal 7 octets


extern log4cpp::Category *badgelog;

//FIXME Cette class gere le lecteur (1 seul pour T2B) pas les badges... A renommer
/**
 * @brief The Badge_qobject class
 */
class Badge_qobject : public QObject
{
    Q_OBJECT

public:
    Badge_qobject(void);
    void init(void);
    QString badge_iso2(QString a);
    QString badge_wiegand(QString a);
    //QString trait_wiegand44b_2s(QBitArray bit);
    //QString trait_tech_talk(QBitArray bit);
    //QString trait_hid_mifare_32bits(QBitArray bit);
    //QString trait_hid_56bits(QBitArray bit);
    //QString trait_wiegand_std(QBitArray bit);
    //QString trait_wiegand_struct(QBitArray bit, sWiegandFormat *wiegform);
    //QString trait_motorola_hid(QBitArray bit);
    //int wieg_chklrc4b(QBitArray bit,char lrc);
    void transmitMsg_stid(); //NOT USED
    int receiveMsg_stid();  //NOT USED
    void stid_legic_send(char *cmd,int timer_value);
    QString trait_data_stid_legic(char * data);
    QString trait_data_inside(char * data);
    QString encode_badge(QString badge);

    QString lecteur;
    QString option;
    QString masque;
    QString format;

    //int badge_debug;
    QString message1,message2;
    QTimer *timer_timeout_morpho;
    int morpho_run;
    QString badgeS;
    QString fileChecksum(const QString &fileName,QCryptographicHash::Algorithm hashAlgorithm);
    QHash<QString, QString> nomPre; //tableau de correspondace
    QString nomPreMd5;
    void ihm_badge(QVariant badge, QVariant jour, QVariant heure, QVariant badgeEncode);

    QThread morpho_thread;
    Morpho morpho;

    // serial
    QString reponse_string;
    QString message;
    QSerialPort *port;
    QString rechercheNomPre(const QString &badge);
    void customEvent(QEvent *event);




public slots:
    void badge_clear_slot();
    void sleep_mode_slot(void); //NOT USED
 //   void init_lecteur(void);

    void morpho_deletealluser(void);
    void morpho_enrolluser(QString);
    void morpho_deleteuserindex(QString);
    void morpho_message(QString);
    void morpho_cancel(void);
    void morpho_destroy_database(void);
    void morpho_create_database(void);
    void morpho_getbaseconfig(void);
    void morpho_reload_database(void);
    void updateNomPre();

signals:

    void ihm_badge(QVariant,QVariant,QVariant,QVariant,QVariant);
    void ihm_badge_inconnu(void);
    void ihm_morpho_status(QVariant); //signal vers ihm database empreintes (0= vide 1=OK)
    void ihm_morpho_enabled(void); //signal vers ihm lecteur Morpho installé (0= vide 1=OK)
    void ihm_morpho_message(QVariant);
    void ihm_badge_message(QVariant);

private:
    int sleep_mode;

    QTimer *timer_init;
    QTimer *timer_attente_badge;
    QTimer *timer_timeout_badge;
    QTimer *timer_timeout_inside;
    QTimer *timer_timeout_stid_legic;
    QTimer *timer_attente_badge_morpho;
    QTimer *timer_timeout_badge_morpho;
    QTimer *_updateNomPre;
    QString badge_encode_morpho;
    QString badgeS_morpho;
    int morpho_egal_badge;// test identifiant empreinte egal numero badge ?

    QFile device;
    QFile badgelu;
    QString badge_encode;
    QSqlDatabase config_db;

    // Variiables lecteur serie
    int dialog_phase;
    int dialog_erreur;
    int dialog_timeout;
    unsigned char presence_badge;

    static const int techtalk_indP1[22];
    static const int techtalk_indP2[22];

    // Masques pour brassage binaire decodage Wiegand 44b => 9 decimal 2S
    static const unsigned long masque_2s[24];


    /* Format HID Corporate 1000, Numero des bits (de 1 a n) utilises pour le calcul des 2 bits de parité b2 et b35 */
    static const char HIDcorp_nbitPb02[22];
    static const char HIDcorp_nbitPb35[22];




    // Variables Lecteur INSIDE
    int puce_type;
    int mifare_init;
    int cmd_index;

private slots:
    void attente_badge(void);
    void attente_badge_morpho(void);
    void timeout_badge(void);
    void timeout_badge_morpho(void);
    void init0(void);
    QString extraire_matricule(QString badge);
    void dialog_inside(void);
    void dialog_stid_legic(void);
    void dialog_morpho(void);
    void morphoOUwiegand_badge(QString );
    void morphoETwiegand_badge(QString );
    void morpho_stop(void);
    void morpho_start(void);
    //void Debug (QString );


    // masque binaire pour lect->option
#define OPT_NO_MIFARE   0x01  // pas de decodage carte Mifare
#define OPT_NO_14443BP  0x02  // pas de decodage carte ISO 14443 B'
#define OPT_NO_14443B   0x04  // pas de decodage carte ISO 14443 B

    // masque pour detection insertion retrait cartes sur reponse[8]
#define ISO14443A_ON1   0x01  // sur Mifare carte absente si 2 polling consecutifs = pas de carte
#define ISO14443A_ON2   0x02  // si carte maintenue = carte absente 1 polling / 2
#define ISO14443BP_ON   0x04

    //Structure de donnees pour la definition des sequene de set status coupleur Inside

#define INSID_SETSTATUS  0xF4       // code instruction set status
#define INSID_SELECTCARD 0xA4       // code instruction select card
#define INSID_LEN_INSTR        5    // longueur instruction vers coupleur Inside
#define INSID_LEN_SETSTATDATA  1    // longueur donnees cde setstatus

#define INSID_SW1_OK      0x90
#define INSID_SW2_OK      0x00
#define INSID_SW1_NOCARD  0x6A
#define INSID_SW1_CARDNI  0x69
#define INSID_SW2_NOCARD  0x82


    // Attention les valeurs qui suivent servent a indexer le tableau des instructions
#define INSID_SETSTAT_ISO14443A   0   // sequence set status user protocole = Mifare
#define INSID_SETSTAT_ISO14443BP  1   // sequence set status user protocole = ISO14443B'

};

/**
 * @brief The Badge_qthread class
 */
class Badge_qthread : public QThread {
public: 
    void init(void);
    void run();
    Badge_qobject badge;

private:
};
#endif // BADGE_H

//FIXME ne pas utiliser de varaible global
GLOBAL Badge_qthread badge_thread;
