#ifndef X7GESTCLEUSB_H
#define X7GESTCLEUSB_H

#include <QObject>

class X7GestCleUsb : public QObject
{
    Q_OBJECT
public:
    explicit X7GestCleUsb(QObject *parent = nullptr);
    void init();

signals:

public slots:
private:
    bool usbkey_mounted;
};

#endif // X7GESTCLEUSB_H
