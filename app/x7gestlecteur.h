#ifndef X7GESTLECTEUR_H
#define X7GESTLECTEUR_H

#include "x7lecteurrf.h"

#include <QObject>

#include <log4cpp/Category.hh>


class X7GestLecteur : public QObject
{
    Q_OBJECT
public:
    explicit X7GestLecteur(QObject *parent = nullptr);
    static X7GestLecteur * getInstance();
    static X7GestLecteur * instance;
    static log4cpp::Category * log4cpp;

    enum Lecteur
    {
        NO_READER = 0,
        ISO2,               // ISO2 data/clock
        WIEGAND,            // WIEGAND
        STID_LEGIC,         // Lecteur serie rs232 STID legic
        INSIDE,             // Lecteur serie TTL INSIDE CONTACTLESS MG220
        MORPHOouWIEGAND,    // Capteur d'empreintes Sagem Morpho CBM ou lecteur WIEGAND
        MORPHOetWIEGAND     // Capteur d'empreintes Sagem Morpho CBM et lecteur WIEGAND
    };

    enum Option_ISO
    {
        OPT_STANDARD = 0,       /* lecture et decodage format ISO2 */
        OPT_MIFARE_STID_HEXA,   /* numero serie mifare recu en 13 decimal, convertit en 8 hexa */
        OPT_ISO1,               /* lecture et decodage format ISO1 */
        OPT_MIFARE_STID_INV,    /* numero serie mifare recu en 13 decimal, inversion octet fort/faible + conversion 10 decimal */
    };

    enum Option_LEGIC
    {
        LEGIC_CSN_DEC = 0,  // decodage decimal
        LEGIC_CSN_DECINV,   // decodage decimal inverse
        LEGIC_CSN_HEX,      // decodage hexadecimal
        LEGIC_CSN_HEXINV,   // decodage hexadecimal inverse
        LEGIC_STX           // decodage hexadecimal 7 octets
    };


    uint getLecteur() const;
    void setLecteur(const uint &lecteur);

    uint getOption() const;
    void setOption(const uint &option);

    QString getMasque() const;
    void setMasque(const QString &masque);

    QString getFormat() const;
    void setFormat(const QString &format);

    QString extraireMatricule(QString badge);

    void setVersionLecteur( QString versionLecteur);
    QString getVersionLecteur();

signals:
    void ihm_badge(QVariant,QVariant,QVariant,QVariant,QVariant);
    void ihm_badge_inconnu(void);

public slots:

    void initLecteur();

private:
    uint _confLecteur;
    uint _confOption;
    QString _confMasque;
    QString _confFormat;
    //TODO creer une class générique pour les lecteurs
    X7LecteurRF *_lecteurRF;
    QString _versionLecteur;
};

#endif // X7GESTLECTEUR_H
