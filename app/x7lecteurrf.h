/**
 * @file
 * @brief X7LecteurRF class declaration
*******************************************************************/
#ifndef X7LECTEURRF_H
#define X7LECTEURRF_H

#include <QObject>
#include <QTimer>
#include <QCryptographicHash>
#include <QFile>

#include "x7interfaceserie.h"

class X7LecteurRF : public X7InterfaceSerie
{
    Q_OBJECT
public:
    explicit X7LecteurRF(QObject *parent = nullptr);
    void getVersionLecteur();
    enum stat
    {
        X7_WAIT_INIT,
        X7_WAIT_VERSION,
        X7_WAIT_BADGE,
        X7_WAIT_DEFAULT,
        /* commande rp */
        X7_READ_PARAMETER_BAUD_RATE, // 0x00
        X7_READ_PARAMETER_AUTOREAD_DATA_START_PAGE, // 0x01
        X7_READ_PARAMETER_AUTOREAD_DATA_BYTE_NUMBER, // 0x02
        X7_READ_PARAMETER_AUTOREAD_DATA_OFFSET, // 0x03
        X7_READ_PARAMETER_REVERSE_AUTOREAD_DATA_BYTE_ORDER, // 0x04
        X7_READ_PARAMETER_13_56MHZ_AUTOREAD_DATA_DECODE_OUTPUT_FORMAT, // 0x05
        X7_READ_PARAMETER_COMPLEMENTED_UID,  // 0x0A
        X7_READ_PARAMETER_AUTOREAD_DATA_INTERFACE_TYPE, // 0x0B
        X7_READ_PARAMETER_FORCE_MIFARE, // 0x0E
        X7_READ_PARAMETER_AUTOREAD_DATA_FIELD_SEPARATOR, // 0x0F
        X7_READ_PARAMETER_POLLING_SEQUENCE, // 0x10
        X7_READ_PARAMETER_DIRECTION_FOR_INTERNAL_RF, // 0x11
        X7_READ_PARAMETER_GAIN, // 0x12
        X7_READ_PARAMETER_DATA_RATE, // 0x13
        X7_READ_PARAMETER_AUTOREAD_DATA_CARD_TYPE, // 0x15
        X7_READ_PARAMETER_LED_ACTIVITY, // 0x1B
        X7_READ_PARAMETER_DISPLAY_CONTRAST, // 0x1C
        X7_READ_PARAMETER_STANDBY_LED_COLOR, // 0x1E
        X7_READ_PARAMETER_READ_MODE, // 0x30
        X7_READ_PARAMETER_MODE_TIMEOUT, // 0x31
        X7_READ_PARAMETER_CARD_TYPE, // 0x32
        X7_READ_PARAMETER_LAYOUT_SOURCE, // 0x33
        X7_READ_PARAMETER_AUTOREAD_DATA_PAD_LENGTH, // 0x34
        X7_READ_PARAMETER_AUTOREAD_DATA_PAD_ALIGNMENT, // 0x35
        X7_WRITE_PARAMETER_SET_DEAFULT_CONFIGURATION, // 0x4F
        X7_READ_ERROR_CODE, // 0xFF
        /* commande wp */
        X7_WRITE_PARAMETER_REVERSE_AUTOREAD_DATA_BYTE_ORDER, // 0x04
        X7_WRITE_PARAMETER_STANDBY_LED_COLOR, // 0x1E
        X7_WRITE_PARAMETER_AUTOREAD_DATA_BYTE_NUMBER, // 0x02
        X7_WRITE_PARAMETER_DIRECTION, // 0x11
        X7_WRITE_PARAMETER_AUTOREAD_CARD_TYPE, //0x15
        X7_WRITE_PARAMETER_HID_PROX_125KHZ_WIEGAND, //0x18
    };

signals:

    void envoieDataVersGest(QVariant);
    void envoieDataVersPort(QByteArray);
    void ihm_badge(QVariant,QVariant,QVariant,QVariant,QVariant);
    void ihm_badge_inconnu(void);

public slots:

    void autoStart();
    virtual void recoitDataDuPort(QByteArray data);
    virtual void recoitDataDuGest(QVariant);

private:
    stat _stat;
    bool _emitMsg;
    QTimer lecturePortSerie;
    QHash<QString, QString> tableauNomPrenom; //tableau de correspondace
    QString nomPreMd5;

    QString rechercheNomPrenom(const QString &badge);
    void updateTableauNomPrenom();
    QString fileChecksum(const QString &fileName,QCryptographicHash::Algorithm hashAlgorithm);
    void lectureParametrageLecteurRF();
    void setParametrageLecteurRF();
    void setParametrage02();
    void setParametrage11();
    void setParametrage15();
};

#endif // X7LECTEURRF_H
