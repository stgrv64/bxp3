#ifndef X7INTERFACESERIE_H
#define X7INTERFACESERIE_H

#include <QObject>
#include <QVariant>
#include <QByteArray>


class X7InterfaceSerie : public QObject
{
    Q_OBJECT
public:
    explicit X7InterfaceSerie(QObject *parent = nullptr);

signals:

    void envoieDataVersGest(QVariant);
    void envoieDataVersPort(QByteArray);
    void read();

public slots:

    virtual void recoitDataDuPort(QByteArray) = 0;
    virtual void recoitDataDuGest(QVariant) = 0;
};

#endif // X7INTERFACESERIE_H
