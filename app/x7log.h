#ifndef X7LOG_H
#define X7LOG_H

#include <QObject>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

class X7Log : public QObject
{
    Q_OBJECT
public:
    explicit X7Log(QObject *parent = nullptr);

    X7Log *getInstance();
    log4cpp::Category * rootLog;

signals:

public slots:
private:
    static X7Log * instance;
};

#endif // X7LOG_H
