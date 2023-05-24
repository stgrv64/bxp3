/**
 * @file
 * @brief X7LecteurRF class definition
 * - Paramétrage options lecteur:
 * - 0: standard
 * - 2: HID 26bit H10301 (8+16 bits to 3+5 decimal digits)
 * - 3: Badge Mifare lecteur Deister wiegand 37 bits
 * - 4: Badge HID 125khz wiegand 37 bits avec code site H10304 (16+19 bits to 5+7 decimal digits)
 * - 5: Badge HID 125khz wiegand 37 bits sans code site H10302 (11 decimal digits)
 * - 11: Badge HID 125khz Corporate 1000 35 bits (4+7 decimal digits)
 * - 13: Lecteur STID Mifare en interface 68 bits GFI, decodage decimal Chrono Gestor
****************************************************************************************************/

#include "x7gestlecteur.h"
#include "x7lecteurrf.h"

#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QApplication>
#include <QRegExp>

log4cpp::Category *x7LecteurRFLog;

X7LecteurRF::X7LecteurRF(QObject *parent) : X7InterfaceSerie(parent)
{
    x7LecteurRFLog = &log4cpp::Category::getInstance("X7LecteurRF");
    x7LecteurRFLog->debug("constructeur");
    _stat = X7_WAIT_INIT;
    _emitMsg = false;
    tableauNomPrenom.clear();
    updateTableauNomPrenom();
    QTimer::singleShot(1000, this, SLOT(autoStart()));
}

void X7LecteurRF::autoStart()
{
    x7LecteurRFLog->debug("autoStart()");
    _stat = X7_WAIT_VERSION;
    _emitMsg = true;
    emit  envoieDataVersPort(QByteArray("v"));
    // la doc precise qu'il faut attendre au minimum 60 msec.
    lecturePortSerie.setInterval(100);
    QObject::connect(&lecturePortSerie, SIGNAL(timeout()), this, SIGNAL(read()));
    lecturePortSerie.start();
    setParametrageLecteurRF();
}

/**
 * @brief X7LecteurRF::recoitDataDuPort
 * @param data
 */
void X7LecteurRF::recoitDataDuPort(QByteArray data)
{
    x7LecteurRFLog->debug("void X7LecteurRF::recoitDataDuPort(QByteArray data)");
    switch(_stat)
    {
    case X7LecteurRF::X7_WAIT_INIT:
        break;
    case X7LecteurRF::X7_WAIT_VERSION:
        x7LecteurRFLog->debug("X7LecteurRF version : " + QString(data).toStdString());
        X7GestLecteur::getInstance()->setVersionLecteur( QString(data));
        _stat = X7_WAIT_DEFAULT;
        _emitMsg = true;
        emit  envoieDataVersPort(QByteArray("wp4fff"));
        break;
    case X7_WAIT_DEFAULT:
    {
        x7LecteurRFLog->debug("X7LecteurRFdefault");
        _stat = X7_WAIT_BADGE;
        _emitMsg = false;
        break;
    }
    case X7LecteurRF::X7_WAIT_BADGE:
    {
        x7LecteurRFLog->debug("badge brut : "+QString(data).toStdString());
        //TODO identifier le pourquoi de cette condition dans les anciennes sources badge.cpp L712
        //if (supervision_thread.supervision.http_etat >0 || supervision_thread.supervision.badge_db_etat >0 )
        QString badge(30);

        badge = QString(data);
        badge.remove(QRegExp("[<>\\r]")); // suppression du caractère < ou > et \r retour chariot
        x7LecteurRFLog->debug("badge sans le sens : "+badge.toStdString());
        uint option = X7GestLecteur::getInstance()->getOption();

        x7LecteurRFLog->debug(QString("badge size : %1").arg(badge.size()).toStdString());
        if(option ==13)
        {
            if(badge.size()!=20)
            {
                x7LecteurRFLog->warn("badge lu : "+badge.toStdString());
                x7LecteurRFLog->warn(QString("badge size : %1").arg(badge.size()).toStdString());
                break;
            }
            if(!QRegExp("^[0-9]+$").exactMatch(badge))
            {
                x7LecteurRFLog->warn("badge lu ne contient pas que des chiffres : "+badge.toStdString());
                break;
            }
            // FIXME problème de décalage au decodage
            badge.remove(0,2);
        }


        QString j=QDate::currentDate().toString(Qt::ISODate);
        QString h=QTime::currentTime().toString(Qt::TextDate);

        //FIXME Badge inconnu ? vraiment ?
        if (!badge.compare("Badge inconnu"))
        {
            emit ihm_badge_inconnu(); //signal vers ihm erreur de bit badge
        }
        else
        {
            badge=X7GestLecteur::getInstance()->extraireMatricule(badge);
            badge=badge.toUpper();
            //Badge encode base64 pour les touches de fonction.
            //TODO faire la conversion dans la partie IHM et supprimer le paramètre.
            QString badge_encode(" "+badge);
            badge_encode = badge_encode.toLatin1().toBase64();
            //TODO Ajouter l'affichage du nom et prenom rechercheNomPre() badge.cpp L743
            QString nomPrenom;
            nomPrenom = rechercheNomPrenom(badge_encode);
            emit ihm_badge((QVariant) badge,(QVariant) j,(QVariant) h,(QVariant) badge_encode, (QVariant) nomPrenom);
        }
        break;
    }
        /**************************************************/
        /*                                                */
        /* COMMANDES rp                                   */
        /*                                                */
        /**************************************************/
    case X7LecteurRF::X7_READ_PARAMETER_BAUD_RATE:
        /****************************/
        /* parameter baud rate 0x00 */
        /****************************/
        if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter baud rate : 9600bps");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter baud rate : 19200bps");
        else if(data.contains("03"))
            x7LecteurRFLog->debug("X7LecteurRF parameter baud rate : 38400bps");
        else if(data.contains("04"))
            x7LecteurRFLog->debug("X7LecteurRF parameter baud rate : 57600bps (default)");
        else if(data.contains("05"))
            x7LecteurRFLog->debug("X7LecteurRF parameter baud rate : 115200bps");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter baud rate : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_AUTOREAD_DATA_START_PAGE;
        emit  envoieDataVersPort(QByteArray("rp01"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_AUTOREAD_DATA_START_PAGE:
        /*******************************************/
        /* parameter autoread data start page 0x01 */
        /*******************************************/
        x7LecteurRFLog->debug("X7LecteurRF autoread data start page : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_AUTOREAD_DATA_BYTE_NUMBER;
        emit  envoieDataVersPort(QByteArray("rp02"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_AUTOREAD_DATA_BYTE_NUMBER:
        /********************************************/
        /* parameter autoread data byte number 0x02 */
        /********************************************/
        x7LecteurRFLog->debug("X7LecteurRF autoread data byte number : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_AUTOREAD_DATA_OFFSET;
        emit  envoieDataVersPort(QByteArray("rp03"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_AUTOREAD_DATA_OFFSET:
        /********************************************/
        /* parameter autoread data offset 0x03      */
        /********************************************/
        x7LecteurRFLog->debug("X7LecteurRF autoread data offset : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_REVERSE_AUTOREAD_DATA_BYTE_ORDER;
        emit  envoieDataVersPort(QByteArray("rp04"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_REVERSE_AUTOREAD_DATA_BYTE_ORDER:
        /***************************************************/
        /* parameter reverse autoread data byte order 0x04 */
        /***************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter Reverse autoread data byte order : LSB byte first (default)");
        else  if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter Reverse autoread data byte order : MSB byte first");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter Reverse autoread data byte order : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_13_56MHZ_AUTOREAD_DATA_DECODE_OUTPUT_FORMAT;
        emit  envoieDataVersPort(QByteArray("rp05"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_13_56MHZ_AUTOREAD_DATA_DECODE_OUTPUT_FORMAT:
        /*************************************************************/
        /* parameter 13.56MHz autoread data decode ouput format 0x05 */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter 13.56Mhz Autoread data decode output format : 2+2+4 bytes to 5+5+10 decimal digits (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter 13.56Mhz Autoread data decode output format : Decode nibbles as BCD / Hex format");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter 13.56Mhz Autoread data decode output format : ASCII as is");
        else if(data.contains("03"))
            x7LecteurRFLog->debug("X7LecteurRF parameter 13.56Mhz Autoread data decode output format : 4 + 4 bytes to 10 + 10 decimal Digits");
        else if(data.contains("0E"))
            x7LecteurRFLog->debug("X7LecteurRF parameter 13.56Mhz Autoread data decode output format : Selected bytes to 20 decimals digits");
        else if(data.contains("0F"))
            x7LecteurRFLog->debug("X7LecteurRF parameter 13.56Mhz Autoread data decode output format : Custom 6 (ISP)");
        else if(data.contains("10"))
            x7LecteurRFLog->debug("X7LecteurRF parameter 13.56Mhz Autoread data decode output format : Memory block content XOR with UID diversification");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter 13.56Mhz Autoread data decode output format : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_COMPLEMENTED_UID;
        emit  envoieDataVersPort(QByteArray("rp0A"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_COMPLEMENTED_UID:
        /*************************************************************/
        /* parameter complemented uid 0x0A                           */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter completed UID : Normal (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter completed UID : Complemented");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter completed UID : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_AUTOREAD_DATA_INTERFACE_TYPE;
        emit  envoieDataVersPort(QByteArray("rp0B"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_AUTOREAD_DATA_INTERFACE_TYPE:
        /*************************************************************/
        /* parameter autoread data interface type 0x0B               */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data interface type : Serial (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data interface type : Clock & data (default)");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data interface type : Wiegand 26 bits");
        else if(data.contains("03"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data interface type : Wiegand 37 bits");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data interface type : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_FORCE_MIFARE;
        emit  envoieDataVersPort(QByteArray("rp0E"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_FORCE_MIFARE:
        /*************************************************************/
        /* parameter autoread force mifare 0x0E                      */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter force Mifare : ISO14443 parts 1-4 card (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter force Mifare : Force JCOP ISO14443A to work as Mifare");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter force Mifare : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_AUTOREAD_DATA_FIELD_SEPARATOR;
        emit  envoieDataVersPort(QByteArray("rp0F"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_AUTOREAD_DATA_FIELD_SEPARATOR:
        /*************************************************************/
        /* parameter autoread autoread data field separator 0x0F     */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data field separator : Disabled (default)");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data field separator : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_POLLING_SEQUENCE;
        emit  envoieDataVersPort(QByteArray("rp10"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_POLLING_SEQUENCE:
        /*************************************************************/
        /* parameter autoread polling sequence 0x10                  */
        /*************************************************************/
        x7LecteurRFLog->debug("X7LecteurRF parameter polling sequence : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_DIRECTION_FOR_INTERNAL_RF;
        emit  envoieDataVersPort(QByteArray("rp11"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_DIRECTION_FOR_INTERNAL_RF:
        /*************************************************************/
        /* parameter autoread direction for internal RF  0x11        */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter direction (for internal RF dual antenna modules only) : Direction enabled (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter direction (for internal RF dual antenna modules only) : direction disabled");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter direction (for internal RF dual antenna modules only) : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_GAIN;
        emit  envoieDataVersPort(QByteArray("rp12"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_GAIN:
        /*************************************************************/
        /* parameter autoread gain  0x12                             */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter gain : 18db");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter gain : 23db");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter gain : 18db");
        else if(data.contains("03"))
            x7LecteurRFLog->debug("X7LecteurRF parameter gain : 23db");
        else if(data.contains("04"))
            x7LecteurRFLog->debug("X7LecteurRF parameter gain : 33db");
        else if(data.contains("05"))
            x7LecteurRFLog->debug("X7LecteurRF parameter gain : 33db (default)");
        else if(data.contains("06"))
            x7LecteurRFLog->debug("X7LecteurRF parameter gain : 43db");
        else if(data.contains("07"))
            x7LecteurRFLog->debug("X7LecteurRF parameter gain : 48db (default)");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter gain : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_DATA_RATE;
        emit  envoieDataVersPort(QByteArray("rp13"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_DATA_RATE:
        /*************************************************************/
        /* parameter autoread data rate  0x13                        */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter data rate : 106Kbit/s (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter data rate : 212Kbit/s");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter data rate : 424Kbit/s");
        else if(data.contains("03"))
            x7LecteurRFLog->debug("X7LecteurRF parameter data rate : 848Kbit/s");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter data rate : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_AUTOREAD_DATA_CARD_TYPE;
        emit  envoieDataVersPort(QByteArray("rp15"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_AUTOREAD_DATA_CARD_TYPE:
        /*************************************************************/
        /* parameter autoread data card type  0x15                   */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data card type : Card type disabled (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data card type : Add char prefix");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data card type : Add char suffix");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data card type : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_LED_ACTIVITY;
        emit  envoieDataVersPort(QByteArray("rp1B"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_LED_ACTIVITY:
        /*************************************************************/
        /* parameter led activity  0x1B                              */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter led activity : Disabled (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter led activity : Enabled");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter led activity : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_DISPLAY_CONTRAST;
        emit  envoieDataVersPort(QByteArray("rp1C"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_DISPLAY_CONTRAST:
        /*************************************************************/
        /* parameter display contrast  0x1C                          */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter display contrast : Min value");
        else if(data.contains("18"))
            x7LecteurRFLog->debug("X7LecteurRF parameter display contrast : Default contrast value");
        else if(data.contains("3F"))
            x7LecteurRFLog->debug("X7LecteurRF parameter display contrast : Max value");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter display contrast : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_STANDBY_LED_COLOR;
        emit  envoieDataVersPort(QByteArray("rp1E"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_STANDBY_LED_COLOR:
        /*************************************************************/
        /* parameter standby led color  0x1E                         */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : LED off");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : green");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : red");
        else if(data.contains("03"))
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : yellow");
        else if(data.contains("04"))
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : blue");
        else if(data.contains("05"))
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : cyan");
        else if(data.contains("06"))
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : magenta");
        else if(data.contains("07"))
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : white");
        else if(data.contains("08"))
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : Led driven externally (default)");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter standby led color : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_READ_MODE;
        emit  envoieDataVersPort(QByteArray("rp30"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_READ_MODE:
        /*************************************************************/
        /* parameter read mode  0x30                                 */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter read mode : Only when card is placed on the reader (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter read mode : In/out");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter read mode : Repetition");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter read mode : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_MODE_TIMEOUT;
        emit  envoieDataVersPort(QByteArray("rp31"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_MODE_TIMEOUT:
        /*************************************************************/
        /* parameter mode timeout  0x31                              */
        /*************************************************************/
        if(data.contains("03"))
            x7LecteurRFLog->debug("X7LecteurRF parameter mode timeout : Default time interval value");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter mode timeout : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_CARD_TYPE;
        emit  envoieDataVersPort(QByteArray("rp32"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_CARD_TYPE:
        /*************************************************************/
        /* parameter card type  0x32                                 */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Disabled (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : All supported (default)");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only Mifare 1K & 4K");
        else if(data.contains("03"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only Mifare 1K");
        else if(data.contains("04"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only Mifare 4K");
        else if(data.contains("05"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only Mifare Ultralight");
        else if(data.contains("06"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : RFU");
        else if(data.contains("07"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only Mifare DESFire");
        else if(data.contains("0E"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only badge codes received from ENTRO app via BLE");
        else if(data.contains("0F"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only badge codes received from a BLE tag");
        else if(data.contains("10"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only 125KHz EM4102");
        else if(data.contains("11"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only 125KHz T5557 (Gymgest)");
        else if(data.contains("12"))
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : Only 125KHz EM4150 (Selesta)");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter card type : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_LAYOUT_SOURCE;
        emit  envoieDataVersPort(QByteArray("rp33"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_LAYOUT_SOURCE:
        /*************************************************************/
        /* parameter layout source  0x33            lecturePortSerie                 */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter layout source : Disabled (default)");
        else if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter layout source : Autoread (default)");
        else if(data.contains("02"))
            x7LecteurRFLog->debug("X7LecteurRF parameter layout source : Biometric");
        else if(data.contains("03"))
            x7LecteurRFLog->debug("X7LecteurRF parameter layout source : Biometric");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter layout source : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_AUTOREAD_DATA_PAD_LENGTH;
        emit  envoieDataVersPort(QByteArray("rp34"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_AUTOREAD_DATA_PAD_LENGTH:
        /*************************************************************/
        /* parameter autoread data pad length  0x34                  */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data pad length : Not used (default)");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data pad length : "+QString(data).toStdString());
        _stat = X7_READ_PARAMETER_AUTOREAD_DATA_PAD_ALIGNMENT;
        emit  envoieDataVersPort(QByteArray("rp35"));
        break;
    case X7LecteurRF::X7_READ_PARAMETER_AUTOREAD_DATA_PAD_ALIGNMENT:
        /*************************************************************/
        /* parameter autoread data pad alignment  0x35               */
        /*************************************************************/
        if(data.contains("00"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data pad alignment : Left padding (default)");
        else         if(data.contains("01"))
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data pad alignment : Right padding");
        else
            x7LecteurRFLog->debug("X7LecteurRF parameter autoread data pad alignment : "+QString(data).toStdString());
        _stat = X7_WAIT_BADGE;
        _emitMsg = false;
        break;
    case X7LecteurRF::X7_WRITE_PARAMETER_SET_DEAFULT_CONFIGURATION:
        /*************************************************************/
        /* parameter set default configuration  0x4F                 */
        /*************************************************************/
        x7LecteurRFLog->debug("X7LecteurRF set default configuration : "+QString(data).toStdString());
        _stat = X7_WAIT_BADGE;
        _emitMsg = false;
        break;
    case X7LecteurRF::X7_READ_ERROR_CODE:
        /*************************************************************/
        /* parameter autoread data pad alignment  0xFF               */
        /*************************************************************/
        x7LecteurRFLog->debug("X7LecteurRF error code : "+QString(data).toStdString());
        _stat = X7_WAIT_BADGE;
        _emitMsg = false;
        break;
    case X7LecteurRF::X7_WRITE_PARAMETER_REVERSE_AUTOREAD_DATA_BYTE_ORDER:
        /*************************************************************/
        /* parameter reverse autoread data byte order 0x04           */
        /*************************************************************/
        x7LecteurRFLog->debug("X7LecteurRF Write param reverse autoread data byte order : "+QString(data).toStdString());
        _stat = X7_READ_ERROR_CODE;
        emit  envoieDataVersPort(QByteArray("rpFF"));
        break;
    case X7LecteurRF::X7_WRITE_PARAMETER_AUTOREAD_DATA_BYTE_NUMBER:
        /*************************************************************/
        /* parameter autoread data byte number                       */
        /*************************************************************/
        x7LecteurRFLog->debug("X7LecteurRF Write param autoread data byte order : "+QString(data).toStdString());
        _stat = X7_READ_ERROR_CODE;
        emit  envoieDataVersPort(QByteArray("rpFF"));
        break;
    case X7LecteurRF::X7_WRITE_PARAMETER_DIRECTION:
        /*************************************************************/
        /* parameter direction                      */
        /*************************************************************/
        x7LecteurRFLog->debug("X7LecteurRF Write param direction : "+QString(data).toStdString());
        _stat = X7_READ_ERROR_CODE;
        emit  envoieDataVersPort(QByteArray("rpFF"));
        break;
    case X7LecteurRF::X7_WRITE_PARAMETER_AUTOREAD_CARD_TYPE:
        /*************************************************************/
        /* parameter autoread data card type                       */
        /*************************************************************/
        x7LecteurRFLog->debug("X7LecteurRF Write param autoread data card type : "+QString(data).toStdString());
        _stat = X7_READ_ERROR_CODE;
        emit  envoieDataVersPort(QByteArray("rpFF"));
        //lectureParametrageLecteurRF();
        break;
    case X7LecteurRF::X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND:
        /*************************************************************/
        /* parameter autoread data card type                       */
        /*************************************************************/
        x7LecteurRFLog->debug("X7LecteurRF Write param HID Prox 125khz wiegand card type : "+QString(data).toStdString());
        _stat = X7_READ_ERROR_CODE;
        emit  envoieDataVersPort(QByteArray("rpFF"));
        //lectureParametrageLecteurRF();
        break;
    }
}

void X7LecteurRF::recoitDataDuGest(QVariant)
{
    x7LecteurRFLog->debug("void X7LecteurRF::recoitDataDuGest(QVariant)");
    //rien pour le moment
}

void X7LecteurRF::lectureParametrageLecteurRF(){
    x7LecteurRFLog->debug("void X7LecteurRF::lectureParametrageLecteurRF()");
    _stat = X7_READ_PARAMETER_BAUD_RATE;
    _emitMsg = true;
    emit  envoieDataVersPort(QByteArray("rp00"));
}

/**
 * @brief X7LecteurRF::setParametrageLecteurRF
 * - options lecteur:
 * - 0: standard
 * - 2: HID 26bit H10301 (8+16 bits to 3+5 decimal digits)
 * - 3: Badge Mifare lecteur Deister wiegand 37 bits
 * - 4: Badge HID 125khz wiegand 37 bits avec code site H10304 (16+19 bits to 5+7 decimal digits)
 * - 5: Badge HID 125khz wiegand 37 bits sans code site H10302 (11 decimal digits)
 * - 11: Badge HID 125khz Corporate 1000 35 bits (4+7 decimal digits)
 * - 13: Lecteur STID Mifare en interface 68 bits GFI, decodage decimal Chrono Gestor
 */
void X7LecteurRF::setParametrageLecteurRF(){
    x7LecteurRFLog->debug("void X7LecteurRF::setParametrageLecteurRF()");
    uint option = 0;
    while(_emitMsg){
        QApplication::processEvents() ;
    }
    if(!_emitMsg){
        x7LecteurRFLog->debug("option = %d", X7GestLecteur::getInstance()->getOption());
        option = X7GestLecteur::getInstance()->getOption();
        switch(option) {
        case 0:
            _stat = X7_WRITE_PARAMETER_REVERSE_AUTOREAD_DATA_BYTE_ORDER;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp0400"));
            setParametrage02();
            // reader
            //
            // wp00 baud rate
            // wp0B interface
            // wp1B activity led
            // wp1E standy led
            // wp13 data rate
            // wp1C contrast
            // wp12 gain
            // wp2E n-drive
            // wp2F p-drive
            // wp10 polling
            // wp15 card type
            // wp30 read mode
            // wp31 mode timeout
            // wp11 direction
            // wp0E javacard
            //
            // card
            //
            // wp32 type
            // wp0D security
            // wp19 125khz format
            // wp33 layout source
            // wp01 start page
            // wp03 offset
            // wp34 pad length
            // wp04 reverse
            // wp22 fileid
            // wp24 file type
            // wp25 comm setting
            // wp14 HCE
            // wp36 erase bio
            // wp37 bio 1
            // wp05 decode output
            // wp02 byte number
            // wp0F separator
            // wp35 pas alignmznt
            // wp0A complement
            // wp21 read key
            // wp23 key slot
            // wp28 read div
            // wp26 write key
            // wp27 key slot
            // wp29 write div
            // wp2A smartcard file
            // wp2B authentication
            break;
        case 2: /*HID 26bit H10301 (8+16 bits to 3+5 decimal digits)*/
            _stat = X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp1803"));

            /* Only badge codes received from the auxiliary HID Prox 125KHz reader */
            _stat = X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp3213"));
            while(_emitMsg){
                QApplication::processEvents() ;
            }
            break;
        case 3: /* Badge Mifare lecteur Deister wiegand 37 bits */
            _stat = X7_READ_ERROR_CODE;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp0500"));
            break;
        case 4: /*Badge HID 125khz wiegand 37 bits avec code site H10304 (16+19 bits to 5+7 decimal digits)*/
            _stat = X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp1805"));

            /* Only badge codes received from the auxiliary HID Prox 125KHz reader */
            _stat = X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp3213"));
            while(_emitMsg){
                QApplication::processEvents() ;
            }
            break;
        case 5: /*Badge HID 125khz wiegand 37 bits sans code site H10302 (11 decimal digits)*/
            _stat = X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp1806"));

            /* Only badge codes received from the auxiliary HID Prox 125KHz reader */
            _stat = X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp3213"));
            while(_emitMsg){
                QApplication::processEvents() ;
            }
            break;
        case 11: /*Badge HID 125khz Corporate 1000 35 bits (4+7 decimal digits)*/
            _stat = X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp1808"));

            /* Only badge codes received from the auxiliary HID Prox 125KHz reader */
            _stat = X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp3213"));
            while(_emitMsg){
                QApplication::processEvents() ;
            }
            break;
        case 13: /* Lecteur STID Mifare en interface 68 bits GFI, decodage decimal Chrono Gestor */

            // lecture uniquement ISO 14443A
            _stat = X7_READ_ERROR_CODE;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp1001"));
            while(_emitMsg){
                QApplication::processEvents() ;
            }

            // decode 8 byte to 20 digit
            _stat = X7_READ_ERROR_CODE;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp050B"));
            while(_emitMsg){
                QApplication::processEvents() ;
            }

            // all supported card
            _stat = X7_READ_ERROR_CODE;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp3201"));
            while(_emitMsg){
                QApplication::processEvents() ;
            }

            // gain +48db
            _stat = X7_READ_ERROR_CODE;
            _emitMsg = true;
            emit  envoieDataVersPort(QByteArray("wp1207"));
            while(_emitMsg){
                QApplication::processEvents() ;
            }
            break;
        }
    }else
        x7LecteurRFLog->debug("");
}

void X7LecteurRF::setParametrage02(){
    x7LecteurRFLog->debug("void X7LecteurRF::setParametrage02()");
    while(_emitMsg){
        QApplication::processEvents() ;
    }
    if(!_emitMsg){
        x7LecteurRFLog->debug("void X7LecteurRF::setParametrage02()");
        _stat = X7_WRITE_PARAMETER_AUTOREAD_DATA_BYTE_NUMBER;
        _emitMsg = true;
        emit  envoieDataVersPort(QByteArray("wp0200"));
        setParametrage15();
    }
}

void X7LecteurRF::setParametrage15(){
    x7LecteurRFLog->debug("void X7LecteurRF::setParametrage15()");
    while(_emitMsg){
        QApplication::processEvents() ;
    }
    if(!_emitMsg){
        x7LecteurRFLog->debug("void X7LecteurRF::setParametrage15()");
        _stat = X7_WRITE_PARAMETER_AUTOREAD_CARD_TYPE;
        _emitMsg = true;
        emit  envoieDataVersPort(QByteArray("wp1500"));
        setParametrage11();
    }
}

void X7LecteurRF::setParametrage11(){
    x7LecteurRFLog->debug("void X7LecteurRF::setParametrage11()");
    while(_emitMsg){
        QApplication::processEvents() ;
    }
    if(!_emitMsg){
        x7LecteurRFLog->debug("void X7LecteurRF::setParametrage11()");
        _stat = X7_WRITE_PARAMETER_DIRECTION;
        _emitMsg = true;
        emit  envoieDataVersPort(QByteArray("wp1100"));
    }
}
QString X7LecteurRF::rechercheNomPrenom(const QString &badge)
{
    x7LecteurRFLog->debug("QString X7LecteurRF::rechercheNomPrenom(const QString &badge)");
    if(badge.isEmpty())
    {
        x7LecteurRFLog->info("rechercheNomPrenom badge vide");
        return QString();
    }else
        if(tableauNomPrenom.isEmpty())
        {
            x7LecteurRFLog->info("rechercheNomPrenom tableauNomPrenom vide");
            return QString();
        }

        else
        {
            x7LecteurRFLog->info("rechercheNomPrenom badge : %s", badge);
            x7LecteurRFLog->debug("rechercheNomPrenom badge : " + badge.toStdString());
            qDebug() << "rechercheNomPrenom badge: " << badge;
            if(tableauNomPrenom.contains(badge))
                return tableauNomPrenom.value(badge);
            else return QString("BADGE\nINCONNU");

        }
}

// correspondance entre les bages et les Nom Prenom pour affichage sur l'image du badge
void X7LecteurRF::updateTableauNomPrenom()
{
    x7LecteurRFLog->debug("updateTableauNomPrenom");
    QString fc;
    QString nom("/var/x7/contents/time2bii.nom");
    fc = fileChecksum(nom,QCryptographicHash::Md5);
    x7LecteurRFLog->debug("void X7LecteurRF::updateTableauNomPrenom() fc = %s ", fc);
    if(fc != nomPreMd5)
    {
        nomPreMd5 = fc;
        tableauNomPrenom.clear();
        QFile fnom(nom);
        if (!fnom.open(QFile::ReadOnly))
        {
            x7LecteurRFLog->debug(fnom.errorString().toStdString());
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
                    tableauNomPrenom.insert(QString(list[0]),n);
                }
            }
            fnom.close();
        }
    }
    x7LecteurRFLog->debug("Liste nom : %d",tableauNomPrenom.size());
}

QString X7LecteurRF::fileChecksum(const QString &fileName,QCryptographicHash::Algorithm hashAlgorithm)
{
    x7LecteurRFLog->debug("fileChecksum");
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

void X7LecteurRF::getVersionLecteur(){
    x7LecteurRFLog->debug("getVersionLecteur");
}
