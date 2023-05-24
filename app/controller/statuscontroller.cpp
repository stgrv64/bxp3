#include "statuscontroller.h"
#include "process.h"
#include "supervision.h"
#include <sys/stat.h>
#include <sys/statfs.h>

#include <QVariant>
#include <QDateTime>
#include <QtXml>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QSqlQuery>

log4cpp::Category * StatusController::log4cpp = &log4cpp::Category::getInstance("StatusController");

StatusController::StatusController()
{
    log4cpp->debug("constructeur");
}

void StatusController::service(HttpRequest& request, HttpResponse& response) {

    log4cpp->debug("service");
    QByteArray method=request.getMethod();
    QByteArray postheader=request.getHeader("Content-Type");
    QByteArray getheader=request.getHeader("Accept");
    response.setHeader("Content-Type",getheader);


    if (method=="GET")
    {
        if (getheader.contains("application/xml"))
            readXml_horodate(request,response);
    }

    else if (method=="POST")
    {
        if (postheader.contains("application/xml"))
            parseXml_horodate(request.getBody(),response);
    }

}


QString StatusController::upTime() const
{
    log4cpp->debug("upTime");
    QFile file("/proc/uptime");
    int uptime = 0;

    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QTextStream textStream(&file);
        QString line;

        do
        {
            line = textStream.readLine().simplified();
            if (!line.isEmpty())
                uptime = (int)line.section(" ", 0, 0).trimmed().toDouble();
        }
        while (!line.isNull());
        file.close();
    }
    return QString("%1d %2h %3m %4s").arg(uptime / 86400).arg(uptime / 3600 % 24).arg(uptime / 60 % 60).arg(uptime % 60);
}

int StatusController::average() const
{
    log4cpp->debug("average");
    int avg = 100;

    QFile file("/proc/loadavg");
    if ( file.open(QIODevice::ReadOnly) )
    {
        QTextStream textStream(&file);
        QString line = textStream.readLine().simplified();
        if (!line.isEmpty())
        {
            avg = (int)(line.section(" ", 0, 0).trimmed().toDouble() * 100.0);
        }
        file.close();
    }

    return avg;
}

QString StatusController::connect2Server() const
{
    log4cpp->debug("connect2Server");
    if(supervision_thread.supervision.http_etat >0)
        return QString("true");
    else
        return QString("false");
}

QString StatusController::freeMemory() const
{
    log4cpp->debug("freeMemory");
    static QTime lastUpdate = QTime::currentTime().addSecs(-3);
    static QString cachedValue = QString("0Mo");

    if ( qAbs( lastUpdate.secsTo( QTime::currentTime() ) ) <= 2 )
        return cachedValue;

    // if /proc/meminfo doesn't exist, return MEMORY FULL
    QFile memFile( "/proc/meminfo" );
    if ( !memFile.open( QIODevice::ReadOnly ) )
        return 0;

    // read /proc/meminfo and sum up the contents of 'MemFree', 'Buffers'
    // and 'Cached' fields. consider swapped memory as used memory.
    qulonglong memoryFree = 0;
    qulonglong memoryTotal = 0;

    QString entry;
    QTextStream readStream( &memFile );
    static const int nElems = 2;
    QString names[nElems] = { "MemFree:", "MemTotal:" };
    qulonglong values[nElems] = { 0, 0 };
    bool foundValues[nElems] = { false, false };
    while ( true )
    {
        entry = readStream.readLine();
        if ( entry.isNull() ) break;
        for ( int i = 0; i < nElems; ++i )
        {
            if ( entry.startsWith( names[i] ) )
            {
                values[i] = entry.section( ' ', -2, -2 ).toULongLong( &foundValues[i] );
            }
        }
    }
    memFile.close();
    bool found = true;
    for ( int i = 0; found && i < nElems; ++i )
        found = found && foundValues[i];
    if ( found )
    {
        //memoryFree = values[0] + values[1] + values[2] + values[3];
        //if ( values[4] > memoryFree )
        //    memoryFree = 0;
        //else
        //    memoryFree -= values[4];
        memoryFree = values[0];
        memoryTotal = values[1];
    }

    lastUpdate = QTime::currentTime();
    return cachedValue = QString("%1Ko/%2Ko").arg(memoryFree).arg(memoryTotal);
}

QString StatusController::freeDiskMemory() const
{
    log4cpp->debug("freeDiskMemory");
    double fKB = 1024;
    struct stat stst;
    struct statfs stfs;
    QString sDirPath("/");
    double fTotal;
    double fFree;

    if ( ::stat(sDirPath.toLocal8Bit(),&stst) == -1 )
        return QString("full1");

    if ( ::statfs(sDirPath.toLocal8Bit(),&stfs) == -1 )
        return QString("full2");

    fFree = stfs.f_bavail * ( stst.st_blksize / fKB );
    fTotal = stfs.f_blocks * ( stst.st_blksize / fKB );


    return QString("%1Ko/%2Ko").arg(fFree).arg(fTotal);
}


int StatusController::badgeStored() const
{
    log4cpp->debug("badgeStored");
    QSqlDatabase badge_db = QSqlDatabase::addDatabase("QSQLITE","badge_form");
    badge_db.setDatabaseName("/var/x7/contents/database_badge.db");
    int nb=0;

    if (badge_db.open())
    {
        QSqlQuery query("SELECT count(*) from badge",badge_db);
        if(query.next())
        {
            nb = query.value(0).toInt();
        }
        badge_db.close();
    }
    return nb;
}

void StatusController::readXml_horodate(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("readXml_horodate");
    QDomDocument doc;

    // Création de l'en-tête du fichier XML : <?xml version="1.0" encoding="UTF-8"?>
    QDomNode xmlNode = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");

    // On insère cet en-tête avant le premier enfant
    doc.insertBefore(xmlNode, doc.firstChild());

    // Création de l'élément <root>
    QDomElement root = doc.createElement("root");

    // On ajoute l'élément <root> en tant que premier enfant de notre document
    doc.appendChild(root);

    //Création de l'élément <status>
    QDomElement status = doc.createElement("status");

    // On ajoute l'élément <horodate> en tant qu'enfant de l'élément <root>
    root.appendChild(status);

    // Création des l'éléments <nom>
    QDomElement uptime = doc.createElement("nom");
    QDomElement freememory = doc.createElement("nom");
    QDomElement freediskmemory = doc.createElement("nom");
    QDomElement sysLoad = doc.createElement("nom");
    QDomElement versionApp = doc.createElement("nom");
    QDomElement badgestored = doc.createElement("nom");
    QDomElement connect2server = doc.createElement("nom");


    uptime.setAttribute( "key","uptime");
    freememory.setAttribute("key","free memory");
    freediskmemory.setAttribute("key","free disk memory");
    sysLoad.setAttribute( "key","sys load");
    versionApp.setAttribute( "key","version app");
    badgestored.setAttribute( "key","badge stored");
    connect2server.setAttribute( "key","connected to server");


    // Ajout de l'attibut "value" qui aura la valeur "xx" à l'élément <nom>
    uptime.setAttribute( "value",upTime());
    freememory.setAttribute( "value",freeMemory());
    freediskmemory.setAttribute( "value",freeDiskMemory());
    sysLoad.setAttribute( "value",average());
    versionApp.setAttribute( "value",supervision_thread.supervision.version_logiciel);
    badgestored.setAttribute( "value",badgeStored());
    connect2server.setAttribute( "value",connect2Server());

    // On ajoute l'élément <nom> en tant qu'enfant de l'élément <horodate>
    status.appendChild(uptime);
    status.appendChild(freememory);
    status.appendChild(freediskmemory);
    status.appendChild(sysLoad);
    status.appendChild(versionApp);
    status.appendChild(badgestored);
    status.appendChild(connect2server);


    response.write(doc.toByteArray());
    doc.clear();
}

void StatusController::parseXml_horodate(const QByteArray &data,HttpResponse& response)
{
    log4cpp->debug("parseXml_horodate");
    response.setStatus(200,"OK");
    response.write("Status");

}
