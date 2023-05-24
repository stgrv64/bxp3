#include <QDebug>
#include "x7carteio.h"
#include "x7customevent.h"
#include "x7gestconfig.h"
#include "x7gestlecteur.h"
#include "x7portserie.h"

X7CarteIO * X7CarteIO::instance = 0;
log4cpp::Category * X7CarteIO::log4cpp = &log4cpp::Category::getInstance("X7CarteIO");


X7CarteIO::X7CarteIO(QObject *parent) : X7InterfaceSerie(parent)
{
    log4cpp->debug("constructeur");
    _version = "";
}

X7CarteIO *X7CarteIO::getInstance()
{
    log4cpp->debug("getInstance");
    if ( !X7CarteIO::instance )
    {
        X7CarteIO * carteIO = new X7CarteIO();
        X7CarteIO::instance = carteIO;
    }
    return X7CarteIO::instance;
}

void X7CarteIO::initCarteIOPort()
{
    log4cpp->debug("initCarteIOPort");
    X7GestConfig::getInstance()->readConfig();
    X7PortSerie *port = X7GestConfig::getInstance()->getPortSerieServiceDevice();
    if(port==nullptr)
        return;
    port->connecte(this);
    log4cpp->debug("portName : " + QString(port->getPortName()).toStdString());
     _stat = X7_GET_VERSION;
    _emitMsg = true;
    _version="";
    emit  envoieDataVersPort(QByteArray("v"));
    // la doc precise qu'il faut attendre au minimum 60 msec.
    lecturePortSerie.setInterval(100);
    QObject::connect(&lecturePortSerie, SIGNAL(timeout()), this, SIGNAL(read()));
    lecturePortSerie.start();
}

void X7CarteIO::recoitDataDuPort(QByteArray data)
{
    log4cpp->debug("recoitDataDuPort");
    //Ici gestion des messages recu par le port série
    switch(_stat)
    {
    case X7CarteIO::X7_GET_VERSION:
        log4cpp->debug("version : " + QString(data).toStdString());
        _version = QString(data);
        _emitMsg = false;
        getPowerLevel(false);
        break;
    case X7CarteIO::X7_GET_POWER_LEVEL:
        log4cpp->debug("power level : " + QString(data).toStdString());
        _powerLevel = QString(data);
        _emitMsg = false;
        //setBlueLight(true);
        break;
    case X7CarteIO::X7_SET_BLUE_LIGHT:
        log4cpp->debug("set blue light : " + QString(data).toStdString());
        _emitMsg = false;
        //setBrightness(50);
        break;
    case X7CarteIO::X7_SET_BRIGHTNESS:
        log4cpp->debug("set brightness : " + QString(data).toStdString());
        _emitMsg = false;
        //getIOState();
        break;
    case X7CarteIO::X7_GET_IO_STATE:
        log4cpp->debug("get io state : " + QString(data).toStdString());
        _ioState = QString(data);
        _emitMsg = false;
        //setIOState(1,2,2);
        break;
    case X7CarteIO::X7_SET_IO_STATE:
        log4cpp->debug("set io state : " + QString(data).toStdString());
        _emitMsg = false;
        break;
    }
}

void X7CarteIO::recoitDataDuGest(QVariant)
{
    log4cpp->debug("recoitDataDuGest");
    //Pas encore de gestion des messages du gest
}


void X7CarteIO::customEvent(QEvent *event)
{
    log4cpp->debug("customEvent");
    if(X7CustomEvent::custom_type()== event->type())
    {
        X7CustomEvent *e = (X7CustomEvent *)event;
        switch (e->getEventType())
        {
        //ajouter les évenements
        default:
            break;

        }
    }
}
void X7CarteIO::autoStart()
{
    log4cpp->debug("autoStart");
    getVersion();
    lecturePortSerie.setInterval(1000);
    QObject::connect(&lecturePortSerie, SIGNAL(timeout()), this, SIGNAL(read()));
    lecturePortSerie.start();
}

void X7CarteIO::getVersion(){
    log4cpp->debug("getVersion");
    _stat = X7_GET_VERSION;
    _emitMsg = true;
    emit  envoieDataVersPort(QByteArray("v"));
}

QString X7CarteIO::getVersionCarteIO(){
    log4cpp->debug("getVersionCarteIO " + _version.toStdString());
    return _version;
}

void X7CarteIO::setBlueLight(bool led){
    log4cpp->debug("setBlueLight");
    _stat = X7_SET_BLUE_LIGHT;
    _emitMsg = true;
    if(led)
        emit  envoieDataVersPort(QByteArray("e1"));
    else
        emit  envoieDataVersPort(QByteArray("e0"));
}

void X7CarteIO::setBrightness(uint brightness){
    log4cpp->debug("setBrightness");
    if(brightness < 1)
        brightness = 1;
    if(brightness > 25)
        brightness = 25;
    _stat = X7_SET_BRIGHTNESS;
    _emitMsg = true;
    QByteArray ba;
    QString value = QString::number(brightness).rightJustified(2, '0');
    QString send = "d" + value;
    ba +=send;
    log4cpp->debug("setBrightness" + send.toStdString());
    emit  envoieDataVersPort(ba);
}

void X7CarteIO::getPowerLevel(int level){
    log4cpp->debug("getPowerLevel");
    _stat = X7_GET_POWER_LEVEL;
    _emitMsg = true;
    if(level==0)
        emit  envoieDataVersPort(QByteArray("adc0"));
    else
        if(level==1)
            emit  envoieDataVersPort(QByteArray("adc1"));
        else
            emit  envoieDataVersPort(QByteArray("adc2"));
}

void X7CarteIO::getIOState(){
    log4cpp->debug("getIOState");
    _stat = X7_GET_IO_STATE;
    _emitMsg = true;
    emit  envoieDataVersPort(QByteArray("%I"));
}

QString X7CarteIO::getIOStateResult(){
    log4cpp->debug("getIOStateResult " + _ioState.toStdString());
    return _ioState;
}

void X7CarteIO::setIOState(int relay, int state, int timeOut){
    log4cpp->debug("setIOState");
    _stat = X7_SET_IO_STATE;
    _emitMsg = true;
    QByteArray ba;
    QString sRelay = QString::number(relay).rightJustified(1, '0');
    QString sState = QString::number(state).rightJustified(1, '0');
    QString sTimeOut = QString::number(timeOut).rightJustified(3, '0');
    QString send = "%O" + sRelay + 0x2C + sState + 0x2C + sTimeOut + 0x0D;
    ba +=send;
    emit  envoieDataVersPort(ba);
}

bool X7CarteIO::getEmitMsg(){
    return _emitMsg;
}

QString X7CarteIO::getPowerLevelResult(){
    log4cpp->debug("getPowerLevelResult " + _powerLevel.toStdString());
    return _powerLevel;
}
