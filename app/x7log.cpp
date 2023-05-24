#include "x7log.h"

X7Log * X7Log::instance = 0;

X7Log::X7Log(QObject *parent) : QObject(parent)
{
    log4cpp::PropertyConfigurator::configure("/var/x7/etc/log4cpp.properties");
    rootLog = &log4cpp::Category::getRoot();
}

X7Log *X7Log::getInstance()
{
    if ( !X7Log::instance )
    {
        X7Log * ins = new X7Log();
        X7Log::instance = ins;
    }

    return X7Log::instance;
}
