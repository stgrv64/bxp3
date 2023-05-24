#ifndef X7PORTSERIE_H
#define X7PORTSERIE_H

#include <QObject>
#include <QByteArray>
#include <QTimer>
#include <log4cpp/Category.hh>

class QSerialPort;
class X7InterfaceSerie;

class X7PortSerie : public QObject
{
    Q_OBJECT
public:    
    enum Device
    {
        X7_INTERNAL_IO,
        X7_READER_1,
        X7_READER_2,
        X7_READER_3,
        X7_READER_4
    };

    explicit X7PortSerie(QObject *parent = nullptr);
    void connecte(X7InterfaceSerie * interfaceSerie);
    void config(Device device);
    QString getPortName();
    static log4cpp::Category * log4cpp;

signals:
    void envoieDataVersGest(QByteArray array);

public slots:
    void recoitDataDuGest(QByteArray array);
    void read();
    void close();

private:
    QSerialPort *_port;
};

#endif // X7PORTSERIE_H
