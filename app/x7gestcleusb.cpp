#include "x7gestcleusb.h"

#include <QProcess>

X7GestCleUsb::X7GestCleUsb(QObject *parent) : QObject(parent)
{
    usbkey_mounted=false;
}

void X7GestCleUsb::init()
{
    //FIXME

    //supression de l'entrèe de la clé Usb (sda1) si elle existe
    QString a= QString("rm /dev/sda1");
    QProcess *qProc = new QProcess(this);
    qProc->start(a);
    qProc->waitForReadyRead();
    QString tmp = qProc->readAll();
    qProc->close();
    qProc->deleteLater();
}
