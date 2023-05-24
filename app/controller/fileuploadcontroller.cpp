/**
  @file
  @author Stefan Frings
*/

#include "fileuploadcontroller.h"
#include <QtXml>
#include <QThread>
#include "badge.h"
#include "T2bEvent.h"

log4cpp::Category * FileUploadController::log4cpp = &log4cpp::Category::getInstance("FileUploadController");

FileUploadController::FileUploadController() {
    log4cpp->debug("constructeur");
}

void FileUploadController::service(HttpRequest& request, HttpResponse& response)
{
    log4cpp->debug("service");
    QByteArray method=request.getMethod();
    QByteArray postheader=request.getHeader("Content-Type");
    QByteArray getheader=request.getHeader("Accept");
    response.setHeader("Content-Type",getheader);

    log4cpp->debug("method : " + method);
    log4cpp->debug("postheader : " + postheader);
    log4cpp->debug("getheader : " + getheader);

    QString j=QDate::currentDate().toString(Qt::ISODate);
    QString h=QTime::currentTime().toString(Qt::TextDate);
    QString pwd=QString("%1%2").arg(h.section(":",0,0)).arg(h.section(":",1,1));

    if (method=="GET")
    {

        QString filename= QString((request.getParameter("Key")));
        QString passwd= QString((request.getParameter("Pswd")));
        if (!pwd.compare(passwd))
        {
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly))
            {
                QDataStream in1(&file);

                // Read characters from file and put into bytearray.
                QByteArray byteArray;
                while(!in1.atEnd()) {
                    char *c = new char[1];
                    in1.readRawData(c,1);
                    byteArray.push_back(*c);
                }
                file.close();
                response.write(byteArray,true);
            }
            else  response.setStatus(564,file.errorString().toLatin1());

        }
        else response.setStatus(464,"Upload non valide");

    }

    else if (method=="POST")
    {
        if (QString(postheader).contains("multipart/form-data",Qt::CaseSensitive))
        {
            bool ret=0;
            QTemporaryFile* file;
            QByteArray fileUpload= request.getParameter("fileUpload");
            log4cpp->debug("*************************************************");
            log4cpp->debug("fileUpload : "+QString(fileUpload).toStdString());
            log4cpp->debug("*************************************************");
            if(!QString(fileUpload).isEmpty())
            {
                file=request.getUploadedFile("fileUpload");
                //log4cpp->debug("POST:"+QString(file->readAll().toHex()).toStdString());
                if(QString(fileUpload).contains("time2bii.bio"))
                {
                    QDir dir;
                    dir.mkpath("/var/x7/contents/biometrie");
                    file->remove("/var/x7/contents/biometrie/time2bii.bio");
                    ret=file->copy("/var/x7/contents/biometrie/time2bii.bio");
                }
                else if(QString(fileUpload).contains("time2bii.wpa"))
                {
                    file->remove("/var/x7/etc/wpa_supplicant.conf");
                    ret=file->copy("/var/x7/etc/wpa_supplicant.conf");
                }
                else if(QString(fileUpload).contains("time2bii.wvdial"))
                {
                    file->remove("/var/x7/etc/wvdial.conf");
                    ret=file->copy("/var/x7/etc/wvdial.conf");
                }
                else if(QString(fileUpload).contains("time2bii.webservice"))
                {
                    file->remove("/var/x7/etc/Webservices.ini");
                    ret=file->copy("/var/x7/etc/Webservices.ini");
                }
                else if(QString(fileUpload).contains("time2bii.cert"))
                {
                    QDir dir;
                    dir.mkpath("/var/x7/contents/ssl");
                    file->remove("/var/x7/contents/ssl/my.cert");
                    ret=file->copy("/var/x7/contents/ssl/my.cert");
                }
                else if(QString(fileUpload).contains("time2bii.key"))
                {
                    QDir dir;
                    dir.mkpath("/var/x7/contents/ssl");
                    file->remove("/var/x7/contents/ssl/my.key");
                    ret=file->copy("/var/x7/contents/ssl/my.key");
                }
                else if(QString(fileUpload).contains("time2bii.crypt"))
                {
                    QDir dir;
                    dir.mkpath("/var/x7/contents/ssl");
                    file->remove("/var/x7/contents/ssl/my.crypt");
                    ret=file->copy("/var/x7/contents/ssl/my.crypt");
                }
                else if(QString(fileUpload).contains("time2bii.cryptGnet"))
                {
                    QDir dir;
                    dir.mkpath("/var/x7/contents/ssl");
                    file->remove("/var/x7/contents/ssl/my.cryptGnet");
                    ret=file->copy("/var/x7/contents/ssl/my.cryptGnet");
                }
                else if(QString(fileUpload).contains(".img"))
                {
                    log4cpp->debug("une image");
                    QDir dir;
                    dir.mkpath("/var/x7/contents/images");
                    log4cpp->debug("mkdir");
                    QString a("/var/x7/contents/images/");
                    a.append(QString(fileUpload));
                    file->remove(a);
                    log4cpp->debug("remove"+ QString(a).toStdString());
                    log4cpp->debug(QString("size %1").arg(file->size()).toStdString());
                    ret=file->copy(a);
                    log4cpp->debug("copy");
                }
                else if(QString(fileUpload).contains(".mp4"))
                {
                    QDir dir;
                    dir.mkpath("/var/x7/contents/mp4");
                    QString a("/var/x7/contents/mp4/");
                    a.append(QString(fileUpload));
                    file->remove(a);
                    ret=file->copy(a);
                }
                else if(QString(fileUpload).contains(".wav"))
                {
                    QDir dir;
                    dir.mkpath("/var/x7/contents/wav");
                    QString a("/var/x7/contents/wav/");
                    a.append(QString(fileUpload));
                    file->remove(a);
                    ret=file->copy(a);
                }
                else if(QString(fileUpload).contains(".m4a"))
                {
                    QDir dir;
                    dir.mkpath("/var/x7/contents/m4a");
                    QString a("/var/x7/contents/m4a/");
                    a.append(QString(fileUpload));
                    file->remove(a);
                    ret=file->copy(a);
                }
                else if(QString(fileUpload).contains("time2bii.nom"))
                {
                    file->remove("/var/x7/contents/time2bii.nom");
                    ret=file->copy("/var/x7/contents/time2bii.nom");

                    QCoreApplication::postEvent((QObject*)&badge_thread.badge,new T2bEvent(EventType::BADGE_UPDATE_NOM));
                    log4cpp->debug("fichier time2bii.nom telecharge");
                }
                else
                {
                    QString fpath= QString((request.getParameter("Key")));
                    QString passwd= QString((request.getParameter("Pswd")));
                    fpath.append("/");
                    if (!pwd.compare(passwd))
                    {
                        fpath.append(fileUpload);
                        file->remove(fpath);
                        ret=file->copy(fpath);
                    }
                    else
                    {
                        ret = false;

                    }

                }
                if (ret)
                {
                    log4cpp->debug("FILEUPLOADED OK");
                    response.write("Upload ok");
                }
                else
                {
                    log4cpp->error("FILEUPLOADED KO");
                    response.setStatus(564,file->errorString().toLatin1());
                }
            }
            else
            {
                log4cpp->error("FILEUPLOADED KO");
                response.setStatus(464,"Upload non valide");
            }
        }
    }
}
