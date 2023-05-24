/*******************************************************************
 * @file x7gestlecteur.cpp
 * @brief Define X7GestLecteur class
*******************************************************************/
#include "x7gestconfig.h"
#include "x7gestlecteur.h"
#include "x7portserie.h"



X7GestLecteur * X7GestLecteur::instance = 0;
log4cpp::Category * X7GestLecteur::log4cpp = &log4cpp::Category::getInstance("x7GestLecteur");

X7GestLecteur::X7GestLecteur(QObject *parent) : QObject(parent)
{
    log4cpp->debug("constructeur");
    _lecteurRF = nullptr;
}

X7GestLecteur *X7GestLecteur::getInstance()
{
    log4cpp->debug("getInstance");
    if ( !X7GestLecteur::instance )
    {
        X7GestLecteur * gestLecteur = new X7GestLecteur();
        X7GestLecteur::instance = gestLecteur;
    }

    return X7GestLecteur::instance;
}


void X7GestLecteur::initLecteur()
{
    log4cpp->debug("initLecteur");
    X7GestConfig::getInstance()->readConfig();
    X7PortSerie *port = X7GestConfig::getInstance()->getPortSerieLecteur();
    if(port==nullptr)
        return;

    //dans le cas d'une mise ajour on supprime le lecteur avant.
    if(_lecteurRF!=nullptr)
    {
        //deleteLater va se charger de déconnecter les signaux.
       _lecteurRF->deleteLater();
       _lecteurRF=nullptr;
    }

    switch(static_cast<Lecteur>(_confLecteur))
    {
    case X7GestLecteur::NO_READER:
        break;
    case X7GestLecteur::ISO2:
        break;
    case X7GestLecteur::WIEGAND:

        // TODO gerer les lecteurs avec une class générique ?!
        _lecteurRF = new X7LecteurRF();
        //Connection des signaux
        port->connecte(_lecteurRF);
        //TODO faire mieux quand on aura plus de temps
        QObject::connect(_lecteurRF,SIGNAL(ihm_badge(QVariant,QVariant,QVariant,QVariant,QVariant)),this,SIGNAL(ihm_badge(QVariant,QVariant,QVariant,QVariant,QVariant)));
        QObject::connect(_lecteurRF,SIGNAL(ihm_badge_inconnu(void)),this,SIGNAL(ihm_badge_inconnu(void)));

        break;
    case X7GestLecteur::STID_LEGIC:
        break;
    case X7GestLecteur::INSIDE:
        break;
    case X7GestLecteur::MORPHOouWIEGAND:
        break;
    case X7GestLecteur::MORPHOetWIEGAND:
        break;

    }
}

QString X7GestLecteur::extraireMatricule(QString badge)
{
    log4cpp->debug("extraireMatricule");
    int i,j;
    QString avant,apres;
    QString matricule;
    matricule=badge;
    if (!_confMasque.isEmpty() && _confMasque.contains('*'))
    {
        matricule.clear();
        i=_confMasque.indexOf("*");
        j=_confMasque.lastIndexOf("*");
        matricule=badge.mid(i,(j-i)+1);
    }

    if (!_confFormat.isEmpty() && _confFormat.contains('*'))
    {
        avant=_confFormat.section('*',0,0);
        apres=_confFormat.section('*',1,1);
        matricule.insert(0,avant);
        matricule.append(apres);
    }
    return(matricule);
}

uint X7GestLecteur::getLecteur() const
{
    return _confLecteur;
}

void X7GestLecteur::setLecteur(const uint &lecteur)
{
    _confLecteur = lecteur;
}

uint X7GestLecteur::getOption() const
{
    return _confOption;
}

void X7GestLecteur::setOption(const uint &option)
{
    _confOption = option;
}

QString X7GestLecteur::getMasque() const
{
    return _confMasque;
}

void X7GestLecteur::setMasque(const QString &masque)
{
    _confMasque = masque;
}

QString X7GestLecteur::getFormat() const
{
    return _confFormat;
}

void X7GestLecteur::setFormat(const QString &format)
{
    _confFormat = format;
}

void X7GestLecteur::setVersionLecteur( QString versionLecteur){
    _versionLecteur = versionLecteur;
}

QString X7GestLecteur::getVersionLecteur(){
    _lecteurRF->getVersionLecteur();
    return _versionLecteur;
}

