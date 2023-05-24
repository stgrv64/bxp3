#include "x7interfaceserie.h"
#include "x7portserie.h"

//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>
#include "qserialport.h"
#include "qserialportinfo.h"
#include <QDebug>
#include <QFile>
#include <QThread>

log4cpp::Category * X7PortSerie::log4cpp = &log4cpp::Category::getInstance("X7PortSerie");

X7PortSerie::X7PortSerie(QObject *parent) : QObject(parent)
{
    log4cpp->debug("constructeur");
    qDebug() << "UtlPortSerie started - thread: " << QThread::currentThreadId();
    _port = new QSerialPort();
}

void X7PortSerie::config(Device device)
{
    log4cpp->debug("config");
    switch(device)
    {
    case X7PortSerie::X7_INTERNAL_IO:
        _port->setPortName("/dev/ttyACM0");
        break;
    case X7PortSerie::X7_READER_1:
        if(QFile::exists("/dev/ttyACM1"))
            _port->setPortName("/dev/ttyACM1");
        else
            _port->setPortName("/dev/ttyUSB0");
        break;
    case X7PortSerie::X7_READER_2:
        _port->setPortName("/dev/ttyUSB1");
        break;
    case X7PortSerie::X7_READER_3:
        _port->setPortName("/dev/ttyUSB2");
        break;
    case X7PortSerie::X7_READER_4:
        _port->setPortName("/dev/ttyUSB3");
        break;
    }

    //les paramètres ci-dessous sont tires de la doc
    //X7 Linux-quick-setup USB v.1.6-ENG.doc
    _port->setBaudRate(QSerialPort::Baud57600);
    _port->setFlowControl(QSerialPort::NoFlowControl);
    _port->setParity(QSerialPort::NoParity);
    _port->setDataBits(QSerialPort::Data8);
    _port->setStopBits(QSerialPort::OneStop);

    if(!_port->open(QIODevice::ReadWrite ))
    {
        qWarning() << _port->portName() <<" erreur ouverture : " << _port->error() << " " << _port->errorString();
    }else
    {
        qDebug() << _port->portName() <<" est ouvert: " << _port->isOpen();

    }

}

QString X7PortSerie::getPortName()
{
    log4cpp->debug("getPortName");
    return _port->portName();
}

void X7PortSerie::read()
{
    //log4cpp->debug("read");
    QByteArray array;
    // la doc precise qu'il faut attendre 60 msec donc utiliser port->waitForReadyRead(60)
    // TODO a tester
    if(_port->bytesAvailable() > 0)
    {
        array = _port->readAll();
        //qDebug() << "X7PortSerie Read : " << array.toHex() << " /" << array.size();
        log4cpp->debug("Read : " + QString(array.toHex()).toStdString() + " " +  " /" + QString::number(array.size()).toStdString());
        emit envoieDataVersGest(array);
    }

}

void X7PortSerie::recoitDataDuGest(QByteArray array)
{
    log4cpp->debug("recoitDataDuGest");
    int send = _port->write(array);
    _port->flush();
    int len = array.length();
    //qDebug() << "Write : " << array.toHex()<< " " << send << "/" << len;
    log4cpp->debug("Write : " + QString(array).toStdString() + " " + QString::number(send).toStdString() + "/" + QString::number(len).toStdString());
    if(send != len)
        log4cpp->error("Erreur ecriture");
}

void X7PortSerie::close()
{
    log4cpp->debug("close");
    log4cpp->debug("Fermeture du port %s",_port->portName());
    _port->close();
    _port->deleteLater();
    deleteLater();
}

void X7PortSerie::connecte(X7InterfaceSerie *interfaceSerie)
{
    log4cpp->debug("connecte");
    // Les trames montantes.
    QObject::connect(this,SIGNAL(envoieDataVersGest(QByteArray)),interfaceSerie,SLOT(recoitDataDuPort(QByteArray)));
    // Les trames descendante.
    QObject::connect(interfaceSerie,SIGNAL(envoieDataVersPort(QByteArray)),this,SLOT(recoitDataDuGest(QByteArray)));
    // Demande de lecture du port série.
    QObject::connect(interfaceSerie,SIGNAL(read()),this,SLOT(read()));
}
