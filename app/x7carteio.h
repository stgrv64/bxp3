#ifndef X7CARTEIO_H
#define X7CARTEIO_H

#include <QObject>
#include <QTimer>


#include <log4cpp/Category.hh>
#include "x7interfaceserie.h"

#define QPOST_EVENT_CARTEIO(event) QCoreApplication::postEvent((QObject*)X7CarteIO::getInstance(),event);

class X7CarteIO : public X7InterfaceSerie
{
    Q_OBJECT
public:
    explicit X7CarteIO(QObject *parent = nullptr);
    static X7CarteIO * getInstance();
    void customEvent( QEvent * event );
    static X7CarteIO * instance;
    void initCarteIOPort();
    QString getVersionCarteIO();
    void getVersion();
    void setBlueLight(bool led);
    void setBrightness(uint brightness);
    void getPowerLevel(int level);
    void getIOState();
    bool getEmitMsg();
    QString getIOStateResult();
    QString getPowerLevelResult();
    void setIOState(int relay, int state, int timeOut);
    enum stat
    {
        X7_WAIT_INIT,
        X7_GET_VERSION,
        X7_GET_POWER_LEVEL,
        X7_SET_BLUE_LIGHT,
        X7_SET_BRIGHTNESS,
        X7_GET_IO_STATE,
        X7_SET_IO_STATE
    };

signals:

    void envoieDataVersGest(QVariant);
    void envoieDataVersPort(QByteArray);

public slots:
    void autoStart();
    virtual void recoitDataDuPort(QByteArray);
    virtual void recoitDataDuGest(QVariant);

private:
    stat _stat;
    bool _emitMsg;
    QString _version;
    QString _powerLevel;
    QString _ioState;
    QTimer lecturePortSerie;
    static log4cpp::Category * log4cpp;
};

#endif // X7CARTEIO_H
